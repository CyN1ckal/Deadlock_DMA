#include "pch.h"
#include "Main Menu.h"
#include "GUI/Color Picker/Color Picker.h"
#include "GUI/Fuser/Fuser.h"
#include "GUI/Fuser/ESP/ESP.h"
#include "GUI/Fuser/Status Bars/Status Bars.h"
#include "GUI/Radar/Radar.h"
#include "GUI/Aimbot/Aimbot.h"
#include "GUI/Keybinds/Keybinds.h"
#include "GUI/Debug GUI/Player List/Player List.h"
#include "GUI/Debug GUI/Class List/Class List.h"
#include "GUI/Debug GUI/Trooper List/Trooper List.h"

void MainMenu::Render()
{
	ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::SeparatorText("Performance");
	if (ImGui::Checkbox("VSync", &bVSync));
	ImGui::SetNextItemWidth(100.0f);
	if (ImGui::SliderInt("Target FPS", &iTargetFPS, 30, 240));

	ImGui::Spacing();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("ImGui IO FPS: %.1f", io.Framerate);
	ImGui::Text("Frame Time: %.3f ms", 1000.0f / io.Framerate);
	ImGui::Text("Delta Time: %.3f ms", io.DeltaTime * 1000.0f);

	ImGui::SeparatorText("Main Features");
	ImGui::Checkbox("Aimbot Settings", &Aimbot::bSettings);
	ImGui::Checkbox("Radar Settings", &Radar::bSettings);
	ImGui::Checkbox("Fuser Settings", &Fuser::bSettings);
	ImGui::Spacing();

	ImGui::SeparatorText("Configuration");
	ImGui::Checkbox("Keybinds", &Keybinds::bSettings);
	ImGui::Checkbox("Color Picker", &ColorPicker::bMasterToggle);

	ImGui::Spacing();

	ImGui::SeparatorText("Debug Tools");
	ImGui::Checkbox("Player List", &PlayerList::bSettings);
	ImGui::Checkbox("Class List", &ClassList::bSettings);
	ImGui::Checkbox("Trooper List", &TrooperList::bSettings);

	ImGui::End();
}