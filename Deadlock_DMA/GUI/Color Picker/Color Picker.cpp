#include "pch.h"
#include "Color Picker.h"
#include "GUI/Fuser/ESP/ESP.h"

void ColorPicker::RenderColorPicker()
{
	ImGui::Begin("Color Picker");

	ImGui::ColorEdit4("Friendly Name Tag Color", &ColorPicker::FriendlyNameTagColor.Value.x);

	ImGui::ColorEdit4("Enemy Name Tag Color", &ColorPicker::EnemyNameTagColor.Value.x);

	ImGui::ColorEdit4("Friendly Bone Color", &ColorPicker::FriendlyBoneColor.Value.x);

	ImGui::ColorEdit4("Enemy Bone Color", &ColorPicker::EnemyBoneColor.Value.x);

	ImGui::ColorEdit4("Sinner's Sacrifice Color", &ColorPicker::SinnersColor.Value.x);

	ImGui::ColorEdit4("Monster Camp Color", &ColorPicker::MonsterCampColor.Value.x);

	ImGui::ColorEdit4("Friendly Radar Color", &ColorPicker::FriendlyRadarColor.Value.x);

	ImGui::ColorEdit4("Enemy Radar Color", &ColorPicker::EnemyRadarColor.Value.x);

	ImGui::ColorEdit4("Friendly Trooper Color", &ColorPicker::FriendlyTrooperColor.Value.x);

	ImGui::ColorEdit4("Enemy Trooper Color", &ColorPicker::EnemyTrooperColor.Value.x);

	ImGui::End();
}
