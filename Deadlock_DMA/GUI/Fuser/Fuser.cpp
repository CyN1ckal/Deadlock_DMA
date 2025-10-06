#include "pch.h"
#include "Fuser.h"
#include "ESP/ESP.h"
#include "Deadlock/Entity List/EntityList.h"

void Fuser::OnFrame()
{
	if (!bFuser) return;

	ImGui::SetNextWindowPos({ 100,100 }, ImGuiCond_FirstUseEver);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 0));

	ImGui::SetNextWindowSize(ScreenSize, ImGuiCond_Always);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;
	ImGui::Begin("Fuser", nullptr, window_flags);

	ESP::OnFrame();

	RenderSoulsPerMinute();

	ImGui::End();

	ImGui::PopStyleColor(1);
}

void Fuser::RenderSettings()
{
	ImGui::Begin("Fuser Settings");

	ImGui::Checkbox("Enable Fuser", &bFuser);

	ImGui::PushItemWidth(60.0f);

	ImGui::InputFloat("Screen Size X", &ScreenSize.x, 0.0f, 0.0f, "%.0f");

	ImGui::InputFloat("Screen Size Y", &ScreenSize.y, 0.0f, 0.0f, "%.0f");

	ImGui::Checkbox("Draw Souls Per Minute", &bDrawSoulsPerMinute);

	ImGui::PopItemWidth();

	ImGui::End();
}

void Fuser::RenderSoulsPerMinute()
{
	if (!bDrawSoulsPerMinute) return;

	std::scoped_lock Lock(EntityList::m_ControllerMutex, Deadlock::m_LocalAddressMutex, Deadlock::m_ServerTimeMutex);

	if (auto It = EntityList::m_PlayerControllers.find(Deadlock::m_LocalPlayerControllerAddress); It != EntityList::m_PlayerControllers.end())
	{
		auto Souls = It->second.m_TotalSouls;
		auto SoulsPerSecond = static_cast<float>(It->second.m_TotalSouls) / Deadlock::m_ServerTime;
		auto SoulsPerMinute = SoulsPerSecond * 60.0f;
		ImGui::PushFont(nullptr, 16.0f);
		ImGui::SetCursorPos({ 2.0f, ScreenSize.y - ImGui::GetTextLineHeight() });
		ImGui::Text("%.1f Souls/Min", SoulsPerMinute);
		ImGui::PopFont();
	}
	else
	{
		return;
	}
}
