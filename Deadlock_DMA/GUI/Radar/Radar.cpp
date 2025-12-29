#include "pch.h"

#include "Radar.h"

#include "Deadlock/Deadlock.h"

#include "Deadlock/Entity List/EntityList.h"

#include "GUI/Color Picker/Color Picker.h"

void Radar::Render()
{
	if (!bDrawRadar) return;

	ZoneScoped;

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(ColorPicker::RadarBackgroundColor));

	ImGui::Begin("Radar");

	DrawEntities();

	ImGui::End();

	ImGui::PopStyleColor(1);
}

void Radar::RenderSettings()
{
	if (!bDrawRadarSettings) return;

	ImGui::Begin("Radar Settings");

	ImGui::Checkbox("Enable Radar", &bDrawRadar);

	ImGui::SliderFloat("Radar Scale", &fRadarScale, 1.0f, 50.0f, "%.1f");

	ImGui::SliderFloat("Ray Size", &fRaySize, 0.0f, 500.0f, "%.1f");

	ImGui::Checkbox("Hide Friendly", &bHideFriendly);

	ImGui::Checkbox("MOBA Style", &bMobaStyle);

	ImGui::End();
}

void Radar::DrawEntities()
{
	ZoneScoped;

	auto DrawList = ImGui::GetWindowDrawList();
	auto WindowPos = ImGui::GetWindowPos();
	auto WindowSize = ImGui::GetWindowSize();

	ImVec2 Center = { WindowPos.x + (WindowSize.x / 2.0f), WindowPos.y + (WindowSize.y / 2.0f) };

	DrawLocalPlayer(DrawList, Center);

	std::scoped_lock Lock(EntityList::m_PawnMutex, EntityList::m_ControllerMutex);

	if (EntityList::m_LocalPawnIndex < 0) return;

	auto& LocalPawn = EntityList::m_PlayerPawns[EntityList::m_LocalPawnIndex];

	Vector3& LocalPlayerPos = LocalPawn.m_Position;

	for (auto& Pawn : EntityList::m_PlayerPawns)
	{
		if (Pawn.IsInvalid() || Pawn.IsDormant() || Pawn.IsLocalPlayer())
			continue;

		Vector3 RawRelativePos = { Pawn.m_Position.x - LocalPlayerPos.x, Pawn.m_Position.y - LocalPlayerPos.y, Pawn.m_Position.z - LocalPlayerPos.z };

		ImVec2 EntityDrawPos = { Center.x - (RawRelativePos.x / fRadarScale), Center.y + (RawRelativePos.y / fRadarScale) };

		// optional, MOBA-only clamp so dots stay inside window
		if (bMobaStyle)
		{
			const ImVec2 radarTL = { WindowPos.x + fRadarPadding, WindowPos.y + fRadarPadding };
			const ImVec2 radarBR = { WindowPos.x + WindowSize.x - fRadarPadding, WindowPos.y + WindowSize.y - fRadarPadding };
			EntityDrawPos = ClampToRect(EntityDrawPos, radarTL, radarBR);
		}

		ImU32 Color = Pawn.IsFriendly() ? ColorPicker::FriendlyRadarColor : ColorPicker::EnemyRadarColor;

		if (bHideFriendly && Pawn.IsFriendly()) continue;

		DrawList->AddCircleFilled(EntityDrawPos, 5.0f, Color);

		// controller lookup stays the same
		auto AssociatedControllerAddr = EntityList::GetEntityAddressFromHandle(Pawn.m_hController);
		if (!AssociatedControllerAddr) continue;

		auto ControllerIt = std::find(
			EntityList::m_PlayerControllers.begin(),
			EntityList::m_PlayerControllers.end(),
			AssociatedControllerAddr
		);

		if (ControllerIt == EntityList::m_PlayerControllers.end()) continue;
		if (ControllerIt->IsInvalid()) continue;
		if (ControllerIt->IsDead()) continue;

		DrawPlayer(*ControllerIt, Pawn, EntityDrawPos);
	}
}

void Radar::DrawLocalPlayer(ImDrawList* DrawList, const ImVec2& Center)
{
	DrawList->AddCircleFilled(Center, 5.0f, IM_COL32(0, 255, 0, 255));

	float Yaw = 0.0f;
	{
		std::scoped_lock Lock(Deadlock::m_ClientYawMutex);
		Yaw = Deadlock::m_ClientYaw;
	}
	Yaw += 180.0f;

	float Rad = DegToRad(Yaw);

	ImVec2 LineEnd = { Center.x + (fRaySize * std::cos(Rad)), Center.y - (fRaySize * std::sin(Rad)) };
	DrawList->AddLine(Center, LineEnd, IM_COL32(0, 255, 0, 255), 2.0f);
}

void Radar::DrawPlayer(const CCitadelPlayerController& PC, const CCitadelPlayerPawn& Pawn, const ImVec2& RadarPos)
{
	if (bHideFriendly && PC.IsFriendly())
		return;

	int LineNumber = 0;
	auto DrawList = ImGui::GetWindowDrawList();
	DrawNameTag(PC, Pawn, DrawList, RadarPos, LineNumber);

	if (bMobaStyle) {
		DrawHealthBar(PC, Pawn, DrawList, RadarPos, LineNumber);
	}
}

void Radar::DrawNameTag(const CCitadelPlayerController& PC, const CCitadelPlayerPawn& Pawn, ImDrawList* DrawList, const ImVec2& AnchorPos, int& LineNumber) {
	std::string text;

	if(bMobaStyle)
		text += std::format("({}) ", PC.m_CurrentLevel);

	text += std::format("{} ", PC.GetHeroName());

	ImVec2 size = ImGui::CalcTextSize(text.c_str());

	ImU32 color = PC.IsFriendly()
		? ColorPicker::FriendlyNameTagColor
		: ColorPicker::EnemyNameTagColor;

	ImVec2 pos = {
		AnchorPos.x - size.x * 0.5f,
		AnchorPos.y + LineNumber * size.y
	};

	DrawList->AddText(pos, color, text.c_str());
	LineNumber++;
}

void Radar::DrawHealthBar(const CCitadelPlayerController& PC, const CCitadelPlayerPawn& Pawn, ImDrawList* DrawList, const ImVec2& AnchorPos, int& LineNumber)
{
	constexpr float HealthBarWidth = 80.0f;
	constexpr float Padding = 2.0f;
	constexpr float UnpaddedWidth = HealthBarWidth - (Padding * 2.0f);

	const float LineH = ImGui::GetTextLineHeight();

	const float maxHp = (PC.m_MaxHealth > 0) ? static_cast<float>(PC.m_MaxHealth) : 1.0f;
	float hpPct = static_cast<float>(PC.m_CurrentHealth) / maxHp;
	if (hpPct < 0.0f) hpPct = 0.0f;
	if (hpPct > 1.0f) hpPct = 1.0f;

	// Bar rect under the anchor, centered
	ImVec2 barTL = { AnchorPos.x - (HealthBarWidth * 0.5f), AnchorPos.y + (LineNumber * LineH) };
	ImVec2 barBR = { barTL.x + HealthBarWidth,             barTL.y + LineH };

	DrawList->AddRectFilled(barTL, barBR, ColorPicker::HealthBarBackgroundColor);

	// Inner (filled) portion
	ImVec2 fillTL = { barTL.x + Padding, barTL.y + Padding };
	ImVec2 fillBR = { fillTL.x + (UnpaddedWidth * hpPct), barBR.y - Padding };
	DrawList->AddRectFilled(fillTL, fillBR, ColorPicker::HealthBarForegroundColor);

	// Health text centered over the bar
	std::string hpText = std::format("{}", PC.m_CurrentHealth);
	ImVec2 textSize = ImGui::CalcTextSize(hpText.c_str());
	ImVec2 textPos = {
		AnchorPos.x - (textSize.x * 0.5f),
		barTL.y + Padding
	};

	DrawList->AddText(textPos, IM_COL32(255, 255, 255, 255), hpText.c_str());

	LineNumber++;
}