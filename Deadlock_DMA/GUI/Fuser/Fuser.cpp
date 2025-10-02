#include "pch.h"
#include "Fuser.h"
#include "ESP/ESP.h"

void Fuser::OnFrame()
{
	ImVec2 pos = bFullScreen ? ImVec2(0, 0) : ImVec2(100, 100);
	ImGui::SetNextWindowPos(pos, bFullScreen ? ImGuiCond_Always : ImGuiCond_FirstUseEver);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 0));

	ImGui::SetNextWindowSize(ScreenSize, ImGuiCond_Always);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;
	ImGui::Begin("Fuser", nullptr, window_flags);

	ESP::OnFrame();

	ImGui::End();

	ImGui::PopStyleColor(1);
}

void Fuser::RenderSettings()
{
	ImGui::Begin("Fuser Settings");

	ImGui::PushItemWidth(100.0f);

	ImGui::InputFloat("Screen Size X", &ScreenSize.x);

	ImGui::InputFloat("Screen Size Y", &ScreenSize.y);

	ImGui::PopItemWidth();

	ImGui::Checkbox("Full Screen", &bFullScreen);

	ImGui::End();
}