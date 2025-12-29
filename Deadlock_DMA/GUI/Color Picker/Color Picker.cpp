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

	MyColorEdit4("Friendly Name Tag", ColorPicker::FriendlyNameTagColor);

	MyColorEdit4("Enemy Name Tag", ColorPicker::EnemyNameTagColor);

	MyColorEdit4("Friendly Bone", ColorPicker::FriendlyBoneColor);

	MyColorEdit4("Enemy Bone", ColorPicker::EnemyBoneColor);

	MyColorEdit4("Sinner's Sacrifice", ColorPicker::SinnersColor);

	MyColorEdit4("Monster Camp", ColorPicker::MonsterCampColor);

	MyColorEdit4("Friendly Radar", ColorPicker::FriendlyRadarColor);

	MyColorEdit4("Enemy Radar", ColorPicker::EnemyRadarColor);

	MyColorEdit4("Friendly Trooper", ColorPicker::FriendlyTrooperColor);

	MyColorEdit4("Enemy Trooper", ColorPicker::EnemyTrooperColor);

	MyColorEdit4("Unsecured Souls Text", ColorPicker::UnsecuredSoulsTextColor);

	MyColorEdit4("Unsecured Souls Highlighted Text", ColorPicker::UnsecuredSoulsHighlightedTextColor);

	MyColorEdit4("Friendly Health Status Bar", ColorPicker::FriendlyHealthStatusBarColor);

	MyColorEdit4("Enemy Health Status Bar", ColorPicker::EnemyHealthStatusBarColor);

	MyColorEdit4("Friendly Souls Status Bar", ColorPicker::FriendlySoulsStatusBarColor);

	MyColorEdit4("Enemy Souls Status Bar", ColorPicker::EnemySoulsStatusBarColor);

	MyColorEdit4("Health Bar Foreground", ColorPicker::HealthBarForegroundColor);

	MyColorEdit4("Health Bar Background", ColorPicker::HealthBarBackgroundColor);

	MyColorEdit4("Aimbot FOV Circle", ColorPicker::AimbotFOVCircle);

	MyColorEdit4("Radar Background", ColorPicker::RadarBackgroundColor);

	ImGui::End();
}
