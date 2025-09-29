#include "pch.h"
#include "Fuser.h"
#include "ESP/ESP.h"

void Fuser::OnFrame()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 0));

	ImGui::SetNextWindowSize({ 1920,1080 }, ImGuiCond_Once);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;
	ImGui::Begin("Fuser", nullptr, window_flags);

	ESP::OnFrame();

	ImGui::End();

	ImGui::PopStyleColor(1);
}