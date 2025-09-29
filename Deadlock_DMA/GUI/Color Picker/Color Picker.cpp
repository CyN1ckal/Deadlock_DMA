#include "pch.h"
#include "Color Picker.h"
#include "GUI/Fuser/ESP/ESP.h"

void ColorPicker::RenderColorPicker()
{
	ImGui::Begin("Color Picker");

	ImGui::ColorEdit4("Friendly Color", &ESP::FriendlyColor.x);

	ImGui::ColorEdit4("Enemy Color", &ESP::EnemyColor.x);

	ImGui::End();
}
