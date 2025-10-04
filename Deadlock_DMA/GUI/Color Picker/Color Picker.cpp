#include "pch.h"
#include "Color Picker.h"
#include "GUI/Fuser/ESP/ESP.h"

void ColorPicker::RenderColorPicker()
{
	ImGui::Begin("Color Picker");

	ImGui::ColorEdit4("Friendly Name Tag Color", &ColorPicker::FriendlyNameTagColor.x);

	ImGui::ColorEdit4("Enemy Name Tag Color", &ColorPicker::EnemyNameTagColor.x);

	ImGui::ColorEdit4("Friendly Bone Color", &ColorPicker::FriendlyBoneColor.x);

	ImGui::ColorEdit4("Enemy Bone Color", &ColorPicker::EnemyBoneColor.x);

	ImGui::ColorEdit4("Sinner's Sacrifice Color", &ColorPicker::SinnersColor.x);

	ImGui::ColorEdit4("Monster Camp Color", &ColorPicker::MonsterCampColor.x);

	ImGui::End();
}
