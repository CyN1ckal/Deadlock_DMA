#include "pch.h"

#include "Camps.h"

#include "Deadlock/Entity List/EntityList.h"

#include "GUI/Color Picker/Color Picker.h"

void Draw_Camps::operator()()
{
	ZoneScoped;

	std::scoped_lock Lock(EntityList::m_MonsterCampMutex);

	auto WindowPos = ImGui::GetWindowPos();
	auto DrawList = ImGui::GetWindowDrawList();

	for (auto& Camp : EntityList::m_MonsterCamps)
	{
		if (Camp.IsInvalid()) continue;

		if (Camp.IsDormant()) continue;

		if (Camp.m_CurrentHealth < 1) continue;

		Vector2 ScreenPos{};
		if (!Deadlock::WorldToScreen(Camp.m_Position, ScreenPos)) continue;

		std::string CampString = std::format("[{}]", Camp.m_CurrentHealth);

		auto TextSize = ImGui::CalcTextSize(CampString.c_str());

		ImGui::SetCursorPos({ ScreenPos.x - (TextSize.x / 2.0f), ScreenPos.y });
		ImGui::TextColored(ColorPicker::MonsterCampColor.Value, CampString.c_str());
	}
}