#include "pch.h"

#include "Sinners.h"

#include "Deadlock/Entity List/EntityList.h"

#include "GUI/Color Picker/Color Picker.h"

void Draw_Sinners::operator()()
{
	ZoneScoped;

	std::scoped_lock Lock(EntityList::m_SinnerMutex);

	auto WindowPos = ImGui::GetWindowPos();
	auto DrawList = ImGui::GetWindowDrawList();

	for (auto& Sinner : EntityList::m_Sinners)
	{
		if (Sinner.IsInvalid()) continue;

		if (Sinner.IsDormant()) continue;

		Vector2 ScreenPos{};
		if (!Deadlock::WorldToScreen(Sinner.m_Position, ScreenPos)) continue;

		std::string SinnerString = std::format("[{}]", Sinner.m_CurrentHealth);

		auto TextSize = ImGui::CalcTextSize(SinnerString.c_str());

		ImGui::SetCursorPos({ ScreenPos.x - (TextSize.x / 2.0f), ScreenPos.y });
		ImGui::TextColored(ColorPicker::SinnersColor.Value, SinnerString.c_str());
	}
}