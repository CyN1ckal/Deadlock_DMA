#include "pch.h"
#include "Color Picker.h"
#include "GUI/Fuser/ESP/ESP.h"

void MyColorEdit4(const char* label, ImColor& color)
{
	ImGui::SetNextItemWidth(150.0f);
	ImGui::ColorEdit4(label, &color.Value.x);
}

void ColorPicker::RenderColorPicker()
{
	ImGui::Begin("Color Picker");

	MyColorEdit4("Friendly Name Tag Color", ColorPicker::FriendlyNameTagColor);

	MyColorEdit4("Enemy Name Tag Color", ColorPicker::EnemyNameTagColor);

	MyColorEdit4("Friendly Bone Color", ColorPicker::FriendlyBoneColor);

	MyColorEdit4("Enemy Bone Color", ColorPicker::EnemyBoneColor);

	MyColorEdit4("Sinner's Sacrifice Color", ColorPicker::SinnersColor);

	MyColorEdit4("Monster Camp Color", ColorPicker::MonsterCampColor);

	MyColorEdit4("Friendly Radar Color", ColorPicker::FriendlyRadarColor);

	MyColorEdit4("Enemy Radar Color", ColorPicker::EnemyRadarColor);

	MyColorEdit4("Friendly Trooper Color", ColorPicker::FriendlyTrooperColor);

	MyColorEdit4("Enemy Trooper Color", ColorPicker::EnemyTrooperColor);

	MyColorEdit4("Unsecured Souls Text Color", ColorPicker::UnsecuredSoulsTextColor);

	MyColorEdit4("Unsecured Souls Highlighted Text Color", ColorPicker::UnsecuredSoulsHighlightedTextColor);

	MyColorEdit4("Friendly Health Status Bar Color", ColorPicker::FriendlyHealthStatusBarColor);

	MyColorEdit4("Enemy Health Status Bar Color", ColorPicker::EnemyHealthStatusBarColor);

	MyColorEdit4("Friendly Souls Status Bar Color", ColorPicker::FriendlySoulsStatusBarColor);

	MyColorEdit4("Enemy Souls Status Bar Color", ColorPicker::EnemySoulsStatusBarColor);

	MyColorEdit4("Health Bar Foreground Color", ColorPicker::HealthBarForegroundColor);

	MyColorEdit4("Health Bar Background Color", ColorPicker::HealthBarBackgroundColor);

	ImGui::End();
}
