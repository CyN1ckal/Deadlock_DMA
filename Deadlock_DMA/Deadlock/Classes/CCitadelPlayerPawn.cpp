#include "pch.h"
#include "CCitadelPlayerPawn.h"
#include "Deadlock/Entity List/EntityList.h"
#include "Deadlock/Const/HeroEnum.hpp"
#include "Deadlock/Const/BoneLists.hpp"
#include "GUI/Color Picker/Color Picker.h"
#include "GUI/Fuser/ESP/ESP.h"	

void CCitadelPlayerPawn::DrawSkeleton(const ImVec2& WindowPos, ImDrawList* DrawList) const
{
	if (!hController.IsValid()) return;

	const auto ControllerAddress = EntityList::GetEntityAddressFromHandle(hController);
	auto& Controller = EntityList::m_PlayerControllers[ControllerAddress];

	auto It = g_HeroBoneMap.find(Controller.m_HeroID);
	if (It == g_HeroBoneMap.end()) return;

	auto SkeletonColor = Controller.IsFriendly() ? ColorPicker::FriendlyBoneColor : ColorPicker::EnemyBoneColor;

	for (const auto& [StartBone, EndBone] : It->second)
	{
		Vector2 Start2D, End2D;

		if (!Deadlock::WorldToScreen(BonePositions[StartBone], Start2D)) continue;
		if (!Deadlock::WorldToScreen(BonePositions[EndBone], End2D)) continue;

		ImVec2 Start = ImVec2(Start2D.x + WindowPos.x, Start2D.y + WindowPos.y);
		ImVec2 End = ImVec2(End2D.x + WindowPos.x, End2D.y + WindowPos.y);
		DrawList->AddLine(Start, End, ImGui::GetColorU32(SkeletonColor), 3.0f);
	}
}

void CCitadelPlayerPawn::DrawNameTag(const ImVec2& WindowPos, ImDrawList* DrawList, CCitadelPlayerController& AssociatedController)
{
	Vector2 ScreenPos{};
	if (!Deadlock::WorldToScreen(Position, ScreenPos)) return;

	std::string NameTagString{};

	if (ESP::NameTagSettings.bShowLevel)	NameTagString += std::format("{0:d} ", AssociatedController.m_CurrentLevel);

	if (ESP::NameTagSettings.bShowHeroName)	NameTagString += std::format("{0:s} ", AssociatedController.GetHeroName());

	if (ESP::NameTagSettings.bShowDistance)	NameTagString += std::format("{0:.0f}m ", this->DistanceFromLocalPlayer());

	if (NameTagString.back() == ' ') NameTagString.pop_back();

	auto TextSize = ImGui::CalcTextSize(NameTagString.c_str());

	auto BackgroundColor = AssociatedController.IsFriendly() ? ColorPicker::FriendlyNameTagColor : ColorPicker::EnemyNameTagColor;

	ImVec2 UpperLeft = ImVec2(ScreenPos.x - (TextSize.x / 2.0f) + WindowPos.x, ScreenPos.y + WindowPos.y);
	ImVec2 LowerRight = ImVec2(ScreenPos.x + (TextSize.x / 2.0f) + WindowPos.x, ScreenPos.y + TextSize.y + WindowPos.y);
	DrawList->AddRectFilled(UpperLeft, LowerRight, ImGui::GetColorU32(BackgroundColor));

	ImGui::SetCursorPos(ImVec2(ScreenPos.x - (TextSize.x / 2.0f), ScreenPos.y));
	ImGui::Text(NameTagString.c_str());
}
