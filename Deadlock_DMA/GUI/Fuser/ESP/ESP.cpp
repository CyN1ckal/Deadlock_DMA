#include "pch.h"

#include "ESP.h"

#include "Deadlock/Entity List/EntityList.h"

void ESP::OnFrame()
{
	std::scoped_lock lock(EntityList::PlayerPawnsMutex);

	auto DrawList = ImGui::GetWindowDrawList();
	auto WindowPos = ImGui::GetWindowPos();

	ImGui::PushFont(nullptr, 18.0f);

	for (auto& [Addr, Pawn] : EntityList::m_PlayerPawns)
	{
		Vector2 ScreenPos{};
		if (!Deadlock::WorldToScreen(Pawn.Position, ScreenPos)) continue;

		std::string HealthString = std::to_string(Pawn.Health);
		auto TextSize = ImGui::CalcTextSize(HealthString.c_str());

		ImGui::SetCursorPos(ImVec2(ScreenPos.x - (TextSize.x / 2.0f), ScreenPos.y));

		ImVec2 UpperLeft = ImVec2(ScreenPos.x - (TextSize.x / 2.0f) + WindowPos.x, ScreenPos.y + WindowPos.y);
		ImVec2 LowerRight = ImVec2(ScreenPos.x + (TextSize.x / 2.0f) + WindowPos.x, ScreenPos.y + TextSize.y + WindowPos.y);

		DrawList->AddRectFilled(UpperLeft, LowerRight, IM_COL32(25, 25, 25, 255));

		ImGui::Text(HealthString.c_str());

		//DrawList->AddCircleFilled(ImVec2(ScreenPos.x + WindowPos.x, ScreenPos.y + WindowPos.y), 5.0f, IM_COL32(255, 0, 0, 255));
	}

	ImGui::PopFont();
}