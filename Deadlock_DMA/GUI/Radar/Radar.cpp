#include "pch.h"
#include "Radar.h"
#include "Deadlock/Deadlock.h"
#include "Deadlock/Entity List/EntityList.h"
#include "GUI/Color Picker/Color Picker.h"
#include "GUI/Utils/ImageLoading.h"
#include "Deadlock/Const/ETeam.h"

void Radar::Render()
{
	if (!bMasterToggle) return;

	ZoneScoped;

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(ColorPicker::RadarBackgroundColor));

	ImGui::Begin("Radar", nullptr, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);

	DrawRadarBackground();

	RenderSettings();

	DrawEntities();

	ImGui::End();

	ImGui::PopStyleColor(1);
}

void Radar::RenderSettings()
{
	ImGui::PushFont(nullptr, 12.0f);
	ImGui::SetCursorPos({ 10.0f, 10.0f });
	ImGui::SetNextItemWidth(50.0f);
	ImGui::SliderFloat("Radar Scale", &fRadarScale, 1.0f, 50.0f, "%.1f");
	ImGui::Checkbox("Hide Friendly", &bHideFriendly);
	ImGui::Checkbox("MOBA Style", &bMobaStyle);
	ImGui::Checkbox("Player-Centered", &bPlayerCentered);
	ImGui::PopFont();
}

ImVec2 Radar::GetLocalPlayerScreenPos(const ImVec2& RadarWindowCenter, const Vector3& RadarCenterGamePos) {
	if (bPlayerCentered) {
		return RadarWindowCenter;
	}
	else {
		auto LocalPlayerPos = EntityList::GetLocalPawnPosition();

		ImVec2 Delta = { LocalPlayerPos.x - RadarCenterGamePos.x, LocalPlayerPos.y - RadarCenterGamePos.y };

		Delta.x /= fRadarScale;
		Delta.y /= fRadarScale;

		return ImVec2(RadarCenterGamePos.x + Delta.x, RadarCenterGamePos.y + Delta.y);
	}
}

Vector3 Radar::FindRadarTopLeftCoords(const Vector3& CenterRadarGamePosition, const ETeam& LocalTeam)
{
	auto RadarSizeGameUnits = GetRadarSizeInGameUnits();

	switch (LocalTeam) {
	case ETeam::ARCH_MOTHER:
		return Vector3{ CenterRadarGamePosition.x + (RadarSizeGameUnits.x / 2.0f), CenterRadarGamePosition.y - (RadarSizeGameUnits.y / 2.0f), 0.0f };
	case ETeam::HIDDEN_KING:
	default:
		return Vector3{ CenterRadarGamePosition.x - (RadarSizeGameUnits.x / 2.0f), CenterRadarGamePosition.y + (RadarSizeGameUnits.y / 2.0f), 0.0f };
	}
}

Vector3 Radar::FindRadarBottomRightCoords(const Vector3& CenterRadarGamePosition, const ETeam& LocalTeam)
{
	auto RadarSizeGameUnits = GetRadarSizeInGameUnits();

	switch (LocalTeam) {
	case ETeam::ARCH_MOTHER:
		return Vector3{ CenterRadarGamePosition.x - (RadarSizeGameUnits.x / 2.0f), CenterRadarGamePosition.y + (RadarSizeGameUnits.y / 2.0f), 0.0f };
	case ETeam::HIDDEN_KING:
	default:
		return Vector3{ CenterRadarGamePosition.x + (RadarSizeGameUnits.x / 2.0f), CenterRadarGamePosition.y - (RadarSizeGameUnits.y / 2.0f), 0.0f };
	}
}

ImVec2 Radar::GetRadarSizeInGameUnits()
{
	auto WindowSize = ImGui::GetWindowSize();
	return ImVec2(WindowSize.x * fRadarScale, WindowSize.y * fRadarScale);
}

ImColor GetRadarColor(const CCitadelPlayerPawn& Pawn) {
	if (Pawn.IsLocalPlayer())
		return ColorPicker::LocalPlayerRadar;

	if (Pawn.m_TeamNum == ETeam::HIDDEN_KING) {
		return ColorPicker::HiddenKingTeamColor;
	}
	else {
		return ColorPicker::ArchMotherTeamColor;
	}
}

void Radar::DrawEntities()
{
	ZoneScoped;

	auto DrawList = ImGui::GetWindowDrawList();
	auto WindowPos = ImGui::GetWindowPos();
	auto WindowSize = ImGui::GetWindowSize();

	const ImVec2 RadarWindowCenter = { WindowPos.x + (WindowSize.x / 2.0f), WindowPos.y + (WindowSize.y / 2.0f) };

	auto LocalPlayerTeam = EntityList::GetLocalPlayerTeam();

	const auto RadarCenterGamePos = GetRadarCenterScreenPos();

	std::scoped_lock Lock(EntityList::m_PawnMutex);

	for (auto& Pawn : EntityList::m_PlayerPawns)
	{
		if (Pawn.IsInvalid() || Pawn.IsDormant())
			continue;

		const Vector3 RawRelativePos = { Pawn.m_Position.x - RadarCenterGamePos.x, Pawn.m_Position.y - RadarCenterGamePos.y, Pawn.m_Position.z - RadarCenterGamePos.z };

		const auto GetFinalScreenPos = [](const Vector3& RawRelativePos, const ImVec2& RadarWindowCenter, float fRadarScale, const ETeam& LocalTeam) -> ImVec2 {
			switch (LocalTeam) {
			case ETeam::ARCH_MOTHER:
				return { RadarWindowCenter.x - (RawRelativePos.x / fRadarScale), RadarWindowCenter.y + (RawRelativePos.y / fRadarScale) };
			case ETeam::HIDDEN_KING:
			default:
				return { RadarWindowCenter.x + (RawRelativePos.x / fRadarScale), RadarWindowCenter.y - (RawRelativePos.y / fRadarScale) };
			}
			};

		const ImVec2 FinalScreenPos = GetFinalScreenPos(RawRelativePos, RadarWindowCenter, fRadarScale, LocalPlayerTeam);

		if (Pawn.IsLocalPlayer()) {
			DrawLocalPlayerViewRay(DrawList, FinalScreenPos, LocalPlayerTeam);
		}

		if (bHideFriendly && Pawn.IsFriendly()) continue;

		DrawList->AddCircleFilled(FinalScreenPos, 5.0f, GetRadarColor(Pawn));

		DrawPlayer(Pawn, FinalScreenPos);
	}
}

void Radar::DrawLocalPlayerViewRay(ImDrawList* DrawList, const ImVec2& ScreenPos, const ETeam& LocalTeam)
{
	float Rad = Deadlock::GetClientYaw() + std::numbers::pi;

	ImVec2 LineEnd = { ScreenPos.x - (fRaySize * std::sin(Rad)), ScreenPos.y - (fRaySize * std::cos(Rad)) };

	if (LocalTeam == ETeam::HIDDEN_KING) {
		LineEnd = { ScreenPos.x + (fRaySize * std::sin(Rad)), ScreenPos.y + (fRaySize * std::cos(Rad)) };
	}

	DrawList->AddLine(ScreenPos, LineEnd, IM_COL32(0, 255, 0, 255), 2.0f);
}

void Radar::DrawPlayer(const CCitadelPlayerPawn& Pawn, const ImVec2& RadarPos)
{
	std::scoped_lock Lock(EntityList::m_ControllerMutex);

	auto PC = EntityList::GetAssociatedPC(Pawn);

	if (!PC)
		return;

	if (bHideFriendly && PC->IsFriendly())
		return;

	int LineNumber = 0;
	auto DrawList = ImGui::GetWindowDrawList();
	DrawNameTag(*PC, Pawn, DrawList, RadarPos, LineNumber);

	if (bMobaStyle) {
		DrawHealthBar(*PC, Pawn, DrawList, RadarPos, LineNumber);
	}
}

void Radar::DrawNameTag(const CCitadelPlayerController& PC, const CCitadelPlayerPawn& Pawn, ImDrawList* DrawList, const ImVec2& AnchorPos, int& LineNumber) {
	ImVec2 TextSize = ImGui::CalcTextSize(PC.GetHeroName().data());

	ImU32 TextColor = GetRadarColor(Pawn);

	ImVec2 FinalPos = { AnchorPos.x - (TextSize.x * 0.5f), AnchorPos.y + (LineNumber * TextSize.y) };

	DrawList->AddText(FinalPos, TextColor, PC.GetHeroName().data());

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

void Radar::DrawRadarBackground() {
	static CTextureInfo RadarBackgroundTexture{};

	if (!RadarBackgroundTexture.pTexture) {
		auto Ret = LoadTextureFromFile("Resources/Map.png");
		if (Ret) {
			RadarBackgroundTexture = *Ret;
		}
	}

	if (!RadarBackgroundTexture.pTexture) return;

	auto DrawList = ImGui::GetWindowDrawList();
	auto WindowPos = ImGui::GetWindowPos();
	auto WindowSize = ImGui::GetWindowSize();

	Vector3 CenterRadarGamePosition = GetRadarCenterScreenPos();

	auto LocalTeam = EntityList::GetLocalPlayerTeam();

	auto TopLeftGameCoords = FindRadarTopLeftCoords(CenterRadarGamePosition, LocalTeam);
	auto BottomRightGameCoords = FindRadarBottomRightCoords(CenterRadarGamePosition, LocalTeam);

	auto UV_TL = GetUVFromCoords(TopLeftGameCoords, LocalTeam);
	auto UV_BR = GetUVFromCoords(BottomRightGameCoords, LocalTeam);

	ImGui::SetCursorPos({ 0.0f, 0.0f });
	ImGui::Image(RadarBackgroundTexture.pTexture, WindowSize, UV_TL, UV_BR);
	ImGui::GetWindowDrawList()->AddRectFilled(WindowPos, { WindowPos.x + WindowSize.x, WindowPos.y + WindowSize.y }, IM_COL32(55, 55, 55, 100));
}

Vector3 Radar::GetRadarCenterScreenPos()
{
	static const Vector3 DefaultCenter{ 0.0f, 0.0f, 0.0f };

	if (bPlayerCentered) {
		return EntityList::GetLocalPawnPosition();
	}

	return DefaultCenter;
}

ImVec2 Radar::GetUVFromCoords(const Vector3& GameCoords, ETeam LocalTeam)
{
	/* In range of (0,MapSize) */
	ImVec2 NormalizedCoords = { GameCoords.x + (MapSize.x / 2.0f), GameCoords.y + (MapSize.y / 2.0f) };

	ImVec2 UV = { NormalizedCoords.x / MapSize.x, 1.0f - (NormalizedCoords.y / MapSize.y) };

	return UV;
}