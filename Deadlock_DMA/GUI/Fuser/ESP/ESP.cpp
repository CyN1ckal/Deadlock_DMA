#include "pch.h"

#include "ESP.h"

#include "Deadlock/Entity List/EntityList.h"

void ESP::OnFrame()
{
	std::scoped_lock lock(FirstEntityList::PlayerPawnsMutex, FirstEntityList::PlayerControllerMutex);

	auto DrawList = ImGui::GetWindowDrawList();
	auto WindowPos = ImGui::GetWindowPos();

	ImGui::PushFont(nullptr, 16.0f);

	for (auto& [Addr, Pawn] : FirstEntityList::m_PlayerPawns)
	{
		auto& Controller = FirstEntityList::m_PlayerControllers[FirstEntityList::GetEntityAddressFromHandle(Pawn.hController)];

		if (Controller.IsDead()) continue;
		if (bHideFriendly && Controller.IsFriendly()) continue;

		Vector2 ScreenPos{};
		if (!Deadlock::WorldToScreen(Pawn.Position, ScreenPos)) continue;

		std::string HealthString = std::format("{0:d} {1:s} {2:d}HP", Controller.m_CurrentLevel, Controller.GetHeroName(), Controller.m_CurrentHealth);
		auto TextSize = ImGui::CalcTextSize(HealthString.c_str());

		auto BackgroundColor = Controller.IsFriendly() ? FriendlyColor : EnemyColor;

		ImVec2 UpperLeft = ImVec2(ScreenPos.x - (TextSize.x / 2.0f) + WindowPos.x, ScreenPos.y + WindowPos.y);
		ImVec2 LowerRight = ImVec2(ScreenPos.x + (TextSize.x / 2.0f) + WindowPos.x, ScreenPos.y + TextSize.y + WindowPos.y);
		DrawList->AddRectFilled(UpperLeft, LowerRight, ImGui::GetColorU32(BackgroundColor));

		ImGui::SetCursorPos(ImVec2(ScreenPos.x - (TextSize.x / 2.0f), ScreenPos.y));
		ImGui::Text(HealthString.c_str());
	}

	ImGui::PopFont();
}

void ESP::RenderSettings()
{
	ImGui::Begin("ESP Settings");

	ImGui::Checkbox("Hide Friendly", &bHideFriendly);

	ImGui::End();
}
