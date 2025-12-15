#include "pch.h"
#include "Fuser.h"
#include "ESP/ESP.h"
#include "Deadlock/Entity List/EntityList.h"
#include "Status Bars/Status Bars.h"
#include "GUI/Aimbot/Aimbot.h"
#include <GUI/Radar/Radar.h>

void Fuser::OnFrame()
{
	if (!bMasterToggle) return;

	ZoneScoped;

	ImGui::SetNextWindowPos({ 100,100 }, ImGuiCond_FirstUseEver);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 0));

	ImGui::SetNextWindowSize(ScreenSize, ImGuiCond_Always);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;
	ImGui::Begin("Fuser", nullptr, window_flags);

	if (Radar::bDrawRadar) {
		Radar::Render();
	}
	else {
		ESP::OnFrame();

		RenderSoulsPerMinute();

		StatusBars::Render();

		Aimbot::RenderFOVCircle();
	}

	ImGui::End();

	ImGui::PopStyleColor(1);
}

void Fuser::RenderSettings()
{
	ImGui::Begin("Fuser Settings");

	ImGui::Checkbox("Enable Fuser", &bMasterToggle);

	ImGui::PushItemWidth(60.0f);

	ImGui::InputFloat("Screen Size X", &ScreenSize.x, 0.0f, 0.0f, "%.0f");

	ImGui::InputFloat("Screen Size Y", &ScreenSize.y, 0.0f, 0.0f, "%.0f");

	ImGui::Checkbox("Draw Souls Per Minute", &bDrawSoulsPerMinute);

	ImGui::Checkbox("Healh Status Bar", &StatusBars::bRenderTeamHealthBar);

	ImGui::Checkbox("Souls Status Bar", &StatusBars::bRenderTeamSoulsBar);

	ImGui::Checkbox("Unspent Souls Status Bar", &StatusBars::bRenderUnspentSoulsBar);

	ImGui::PopItemWidth();

	ImGui::End();
}

void Fuser::RenderSoulsPerMinute()
{
	if (!bDrawSoulsPerMinute) return;

	ZoneScoped;

	std::scoped_lock PawnLock(EntityList::m_ControllerMutex);

	if (EntityList::m_LocalControllerIndex < 0) return;

	auto& LocalController = EntityList::m_PlayerControllers[EntityList::m_LocalControllerIndex];

	auto Souls = LocalController.m_TotalSouls;
	float SoulsPerSecond = 0.0f;

	{
		std::scoped_lock timeLock(Deadlock::m_ServerTimeMutex);
		SoulsPerSecond = static_cast<float>(LocalController.m_TotalSouls) / Deadlock::m_ServerTime;
	}

	auto SoulsPerMinute = SoulsPerSecond * 60.0f;
	ImGui::PushFont(nullptr, 24.0f);
	ImGui::SetCursorPos({ 2.0f, ScreenSize.y - ImGui::GetTextLineHeight() });
	ImGui::Text("%.1f Souls/Min", SoulsPerMinute);
	ImGui::PopFont();
}
