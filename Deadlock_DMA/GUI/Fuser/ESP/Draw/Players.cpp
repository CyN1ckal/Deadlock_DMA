#include "pch.h"
#include "Players.h"
#include "Deadlock/Entity List/EntityList.h"
#include "Deadlock/Const/HeroEnum.hpp"
#include "Deadlock/Const/BoneLists.hpp"
#include "GUI/Color Picker/Color Picker.h"
#include "GUI/Fonts/Fonts.h"
#include "Deadlock/Const/Aimpoints.h"

void Draw_Players::operator()()
{
	ZoneScoped;

	std::scoped_lock lock(EntityList::m_PawnMutex, EntityList::m_ControllerMutex);

	auto WindowPos = ImGui::GetWindowPos();
	auto DrawList = ImGui::GetWindowDrawList();

	for (auto& Pawn : EntityList::m_PlayerPawns)
	{
		if (Pawn.IsInvalid()) continue;

		auto AssociatedControllerAddr = EntityList::GetEntityAddressFromHandle(Pawn.m_hController);

		if (!AssociatedControllerAddr) continue;

		if (bHideLocalPlayer && Pawn.IsLocalPlayer()) continue;

		auto ControllerIt = std::find(EntityList::m_PlayerControllers.begin(), EntityList::m_PlayerControllers.end(), AssociatedControllerAddr);

		if (ControllerIt == EntityList::m_PlayerControllers.end())
			continue;

		if (ControllerIt->IsInvalid()) continue;

		if (ControllerIt->IsDead()) continue;

		DrawPlayer(*ControllerIt, Pawn);
	}
}

void Draw_Players::DrawPlayer(const CCitadelPlayerController& PC, const CCitadelPlayerPawn& Pawn)
{
	if (bHideFriendly && PC.IsFriendly())
		return;

	Vector2 ScreenPos{};
	if (!Deadlock::WorldToScreen(Pawn.m_Position, ScreenPos)) return;

	auto DrawList = ImGui::GetWindowDrawList();
	auto WindowPos = ImGui::GetWindowPos();

	if (bDrawBones)
		DrawSkeleton(PC, Pawn, DrawList, WindowPos);

	if (bDrawHead)
		DrawHeadCircle(PC, Pawn, DrawList, WindowPos);

	if (bDrawVelocityVector)
		DrawVelocityVector(Pawn, DrawList, WindowPos);

	if (bBoneNumbers)
		DrawBoneNumbers(Pawn);

	int LineNumber = 0;

	DrawNameTag(PC, Pawn, DrawList, WindowPos, LineNumber);

	if (bDrawHealthBar)
		DrawHealthBar(PC, Pawn, ImVec2(ScreenPos.x + WindowPos.x, ScreenPos.y + WindowPos.y), DrawList, LineNumber);
}

void Draw_Players::DrawHealthBar(const CCitadelPlayerController& PC, const CCitadelPlayerPawn& Pawn, const ImVec2& PawnScreenPos, ImDrawList* DrawList, int& LineNumber)
{
	constexpr float HealthBarWidth = 80.0f;
	constexpr float Padding = 2.0f;
	constexpr float UnpaddedWidth = HealthBarWidth - (Padding * 2.0f);

	auto TextLineHeight = ImGui::GetTextLineHeight();

	float HealthPercent = static_cast<float>(PC.m_CurrentHealth) / static_cast<float>(PC.m_MaxHealth);

	ImVec2 BarTopLeft = ImVec2(PawnScreenPos.x - (HealthBarWidth / 2.0f), PawnScreenPos.y + (LineNumber * TextLineHeight));
	ImVec2 BarBottomRight = ImVec2(BarTopLeft.x + HealthBarWidth, BarTopLeft.y + TextLineHeight);
	DrawList->AddRectFilled(BarTopLeft, BarBottomRight, ColorPicker::HealthBarBackgroundColor);

	BarTopLeft.x += Padding;
	BarTopLeft.y += Padding;
	BarBottomRight.x = BarTopLeft.x + (UnpaddedWidth * HealthPercent);
	BarBottomRight.y -= Padding;
	DrawList->AddRectFilled(BarTopLeft, BarBottomRight, ColorPicker::HealthBarForegroundColor);

	ImGui::PushFont(nullptr, ImGui::GetTextLineHeight() - Padding);

	std::string HealthText = std::format("{0:d}", PC.m_CurrentHealth);
	auto TextSize = ImGui::CalcTextSize(HealthText.c_str());
	DrawList->AddText(ImVec2(PawnScreenPos.x - (TextSize.x / 2.0f), PawnScreenPos.y + Padding + (LineNumber * TextLineHeight)), IM_COL32(255, 255, 255, 255), HealthText.c_str());

	ImGui::PopFont();

	LineNumber++;
}

void Draw_Players::DrawSkeleton(const CCitadelPlayerController& PC, const CCitadelPlayerPawn& Pawn, ImDrawList* DrawList, const ImVec2& WindowPos)
{
	auto It = g_HeroBoneMap.find(PC.m_HeroID);
	if (It == g_HeroBoneMap.end()) return;

	for (const auto& [StartBone, EndBone] : It->second)
	{
		Vector2 Start2D, End2D;

		if (!Deadlock::WorldToScreen(Pawn.m_BonePositions[StartBone], Start2D)) continue;
		if (!Deadlock::WorldToScreen(Pawn.m_BonePositions[EndBone], End2D)) continue;

		ImVec2 Start = ImVec2(Start2D.x + WindowPos.x, Start2D.y + WindowPos.y);
		ImVec2 End = ImVec2(End2D.x + WindowPos.x, End2D.y + WindowPos.y);
		DrawList->AddLine(Start, End, ColorPicker::SkeletonColor, fBonesThickness);
	}
}

void Draw_Players::DrawHeadCircle(const CCitadelPlayerController& PC, const CCitadelPlayerPawn& Pawn, ImDrawList* DrawList, const ImVec2& WindowPos)
{
	auto HeadBoneIndex = Aimpoints::GetAimpoints(PC.m_HeroID).first;

	Vector2 Head2D;
	if (!Deadlock::WorldToScreen(Pawn.m_BonePositions[HeadBoneIndex], Head2D)) return;

	ImVec2 HeadPos = ImVec2(Head2D.x + WindowPos.x, Head2D.y + WindowPos.y);

	float Distance = Pawn.DistanceFromLocalPlayer(false);
	if (Distance < 0.1f) return;

	float BaseRadius = 5.f;

	// Scale radius inversely with distance
	float DistanceScale = 1000.f / (Distance + 100.f);
	float HeadRadius = BaseRadius * DistanceScale;

	DrawList->AddCircle(HeadPos, HeadRadius, ColorPicker::SkeletonColor, 32, fBonesThickness);
}

void Draw_Players::DrawVelocityVector(const CCitadelPlayerPawn& Pawn, ImDrawList* DrawList, const ImVec2& WindowPos)
{
	Vector3 FuturePosition = Pawn.m_Position + Pawn.m_Velocity;
	Vector2 Start2D, End2D;
	if (!Deadlock::WorldToScreen(Pawn.m_Position, Start2D)) return;
	if (!Deadlock::WorldToScreen(FuturePosition, End2D)) return;
	ImVec2 Start = ImVec2(Start2D.x + WindowPos.x, Start2D.y + WindowPos.y);
	ImVec2 End = ImVec2(End2D.x + WindowPos.x, End2D.y + WindowPos.y);
	DrawList->AddLine(Start, End, ImColor(255, 255, 255), 5.0f);
}

void Draw_Players::DrawBoneNumbers(const CCitadelPlayerPawn& Pawn)
{
	ImGui::PushFont(nullptr, 12.0f);

	for (int i = 0; i < MAX_BONES; i++)
	{
		Vector2 ScreenPos{};
		if (!Deadlock::WorldToScreen(Pawn.m_BonePositions[i], ScreenPos)) continue;

		std::string BoneString = std::to_string(i);
		auto TextSize = ImGui::CalcTextSize(BoneString.c_str());

		ImGui::SetCursorPos(ImVec2(ScreenPos.x - (TextSize.x / 2.0f), ScreenPos.y));
		ImGui::Text(BoneString.c_str());
	}

	ImGui::PopFont();
}

void Draw_Players::DrawNameTag(const CCitadelPlayerController& PC, const CCitadelPlayerPawn& Pawn, ImDrawList* DrawList, const ImVec2& WindowPos, int& LineNumber)
{
	Vector2 ScreenPos{};
	if (!Deadlock::WorldToScreen(Pawn.m_Position, ScreenPos)) return;

	std::string NameTagString{};

	NameTagString += std::format("{0:s} ", PC.GetHeroName());

	if (bShowDistance)
		NameTagString += std::format("[{0:.0f}m] ", Pawn.DistanceFromLocalPlayer(true));

	if (NameTagString.back() == ' ') NameTagString.pop_back();

	auto TextSize = ImGui::CalcTextSize(NameTagString.c_str());

	auto NameTagColor = PC.m_TeamNum == ETeam::HIDDEN_KING ? ColorPicker::HiddenKingTeamColor : ColorPicker::ArchMotherTeamColor;

	ImGui::SetCursorPos(ImVec2(ScreenPos.x - (TextSize.x / 2.0f), ScreenPos.y + (LineNumber * TextSize.y)));
	ImGui::TextColored(NameTagColor, NameTagString.c_str());

	LineNumber++;

	if (bDrawUnsecuredSouls)
		DrawUnsecuredSouls(Pawn, ScreenPos, LineNumber);
}

void Draw_Players::DrawUnsecuredSouls(const CCitadelPlayerPawn& Pawn, const Vector2& ScreenPos, int& LineNumber)
{
	if (Pawn.m_UnsecuredSouls < UnsecuredSoulsMinimumThreshold)
		return;

	ImColor UnsecuredSoulColor = ColorPicker::UnsecuredSoulsTextColor;
	std::string SoulText = std::format("{} ", Pawn.m_UnsecuredSouls);

	if (Pawn.m_UnsecuredSouls > UnsecuredSoulsHighlightThreshold)
	{
		UnsecuredSoulColor = ColorPicker::UnsecuredSoulsHighlightedTextColor;
		SoulText += "UNSECURED";
	}
	else
		SoulText += "Unsecured";

	auto SoulTextSize = ImGui::CalcTextSize(SoulText.c_str());
	ImGui::SetCursorPos(ImVec2(ScreenPos.x - (SoulTextSize.x / 2.0f), ScreenPos.y + (LineNumber * SoulTextSize.y)));
	ImGui::TextColored(UnsecuredSoulColor, SoulText.c_str());

	LineNumber++;
}
