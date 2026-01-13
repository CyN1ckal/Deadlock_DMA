#include "pch.h"
#include "Color Picker.h"
#include "GUI/Fuser/ESP/ESP.h"

void ColorPicker::Render()
{
	if (!bMasterToggle)	return;

	ImGui::Begin("Color Picker", &bMasterToggle);

	MyColorPicker("Friendly Name Tag", ColorPicker::FriendlyNameTagColor);

	MyColorPicker("Enemy Name Tag", ColorPicker::EnemyNameTagColor);

	MyColorPicker("Friendly Bone", ColorPicker::FriendlyBoneColor);

	MyColorPicker("Enemy Bone", ColorPicker::EnemyBoneColor);

	MyColorPicker("Sinner's Sacrifice", ColorPicker::SinnersColor);

	MyColorPicker("Monster Camp", ColorPicker::MonsterCampColor);

	MyColorPicker("Friendly Radar", ColorPicker::FriendlyRadarColor);

	MyColorPicker("Enemy Radar", ColorPicker::EnemyRadarColor);

	MyColorPicker("Friendly Trooper", ColorPicker::FriendlyTrooperColor);

	MyColorPicker("Enemy Trooper", ColorPicker::EnemyTrooperColor);

	MyColorPicker("Unsecured Souls Text", ColorPicker::UnsecuredSoulsTextColor);

	MyColorPicker("Unsecured Souls Highlighted Text", ColorPicker::UnsecuredSoulsHighlightedTextColor);

	MyColorPicker("Friendly Health Status Bar", ColorPicker::FriendlyHealthStatusBarColor);

	MyColorPicker("Enemy Health Status Bar", ColorPicker::EnemyHealthStatusBarColor);

	MyColorPicker("Friendly Souls Status Bar", ColorPicker::FriendlySoulsStatusBarColor);

	MyColorPicker("Enemy Souls Status Bar", ColorPicker::EnemySoulsStatusBarColor);

	MyColorPicker("Health Bar Foreground", ColorPicker::HealthBarForegroundColor);

	MyColorPicker("Health Bar Background", ColorPicker::HealthBarBackgroundColor);

	MyColorPicker("Aimbot FOV Circle", ColorPicker::AimbotFOVCircle);

	MyColorPicker("Radar Background", ColorPicker::RadarBackgroundColor);

	ImGui::End();
}

void ColorPicker::MyColorPicker(const char* label, ImColor& color)
{
	ImGui::SetNextItemWidth(150.0f);
	ImGui::ColorEdit4(label, &color.Value.x);
}