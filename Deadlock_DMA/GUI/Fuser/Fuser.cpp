#include "pch.h"
#include "Fuser.h"
#include "ESP/ESP.h"
#include "Deadlock/Entity List/EntityList.h"
#include "Status Bars/Status Bars.h"

void Fuser::OnFrame()
{
	if (!bMasterToggle) return;

	ZoneScoped;

	ImGui::SetNextWindowPos({ 100,100 }, ImGuiCond_FirstUseEver);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 0));

	ImGui::SetNextWindowSize(ScreenSize, ImGuiCond_Always);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;
	ImGui::Begin("Fuser", nullptr, window_flags);

	ESP::OnFrame();

	RenderSoulsPerMinute();

	StatusBars::Render();

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

	ImGui::PopItemWidth();

	ImGui::End();
}

void Fuser::RenderSoulsPerMinute()
{
	if (!bDrawSoulsPerMinute) return;

	ZoneScoped;

	CCitadelPlayerPawn* pPawn = nullptr;

	std::scoped_lock PawnLock(EntityList::m_PawnMutex);

	{
		std::scoped_lock AddrLock(Deadlock::m_LocalAddressMutex);
		auto PawnIt = std::find(EntityList::m_PlayerPawns.begin(), EntityList::m_PlayerPawns.end(), Deadlock::m_LocalPlayerPawnAddress);
		if (PawnIt == EntityList::m_PlayerPawns.end()) return;
		pPawn = &*PawnIt;
	}

	if (!pPawn) return;

	auto Souls = pPawn->m_TotalSouls;
	float SoulsPerSecond = 0.0f;

	{
		std::scoped_lock timeLock(Deadlock::m_ServerTimeMutex);
		SoulsPerSecond = static_cast<float>(pPawn->m_TotalSouls) / Deadlock::m_ServerTime;
	}

	auto SoulsPerMinute = SoulsPerSecond * 60.0f;
	ImGui::PushFont(nullptr, 24.0f);
	ImGui::SetCursorPos({ 2.0f, ScreenSize.y - ImGui::GetTextLineHeight() });
	ImGui::Text("%.1f Souls/Min", SoulsPerMinute);
	ImGui::PopFont();
}
