#include "pch.h"

#include "Troopers.h"

#include "Deadlock/Entity List/EntityList.h"

#include "GUI/Color Picker/Color Picker.h"

void Draw_Troopers::operator()()
{
	ZoneScoped;

	std::scoped_lock Lock(EntityList::m_TrooperMutex, EntityList::m_ControllerMutex);

	auto WindowPos = ImGui::GetWindowPos();
	auto DrawList = ImGui::GetWindowDrawList();

	for (auto& Trooper : EntityList::m_Troopers)
	{
		if (Trooper.IsInvalid()) continue;

		if (Trooper.IsDormant()) continue;

		auto bIsFriend = Trooper.IsFriendly();

		if (bHideFriendly && bIsFriend)
			continue;

		Vector2 ScreenPos{};
		if (!Deadlock::WorldToScreen(Trooper.m_Position, ScreenPos)) continue;

		std::string TrooperString = std::format("{}", Trooper.m_CurrentHealth);
		auto TextSize = ImGui::CalcTextSize(TrooperString.c_str());
		ImGui::SetCursorPos({ ScreenPos.x - (TextSize.x / 2.0f), ScreenPos.y });
		ImGui::TextColored((Trooper.m_TeamNum == ETeam::HIDDEN_KING) ? ColorPicker::HiddenKingTeamColor : ColorPicker::ArchMotherTeamColor, TrooperString.c_str());
	}
}