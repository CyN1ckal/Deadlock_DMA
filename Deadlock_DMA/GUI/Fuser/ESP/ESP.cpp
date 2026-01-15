#include "pch.h"

#include "ESP.h"

#include "Deadlock/Entity List/EntityList.h"

#include "GUI/Color Picker/Color Picker.h"

#include "Draw/Players.h"
#include "Draw/Troopers.h"
#include "Draw/Camps.h"
#include "Draw/Sinners.h"

void ESP::OnFrame()
{
	if (!bMasterToggle)
		return;

	ZoneScoped;

	auto DrawList = ImGui::GetWindowDrawList();
	auto WindowPos = ImGui::GetWindowPos();

	ImGui::PushFont(nullptr, 16.0f);

	if (Draw_Players::bMasterToggle)
		Draw_Players()();

	if (Draw_Troopers::bMasterToggle)
		Draw_Troopers()();

	if (Draw_Camps::bMasterToggle)
		Draw_Camps()();

	if (Draw_Sinners::bMasterToggle)
		Draw_Sinners()();

	ImGui::PopFont();
}

void ESP::RenderSettings()
{
	ImGui::Begin("ESP Settings");


	ImGui::Checkbox("Enable ESP", &bMasterToggle);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::SeparatorText("Players");

	ImGui::Checkbox("Enable Player ESP", &Draw_Players::bMasterToggle);

	ImGui::Spacing();
	ImGui::Checkbox("Hide Friendly", &Draw_Players::bHideFriendly);
	ImGui::Checkbox("Hide Local", &Draw_Players::bHideLocalPlayer);

	ImGui::Spacing();

	ImGui::Checkbox("Bones", &Draw_Players::bDrawBones);
	if (Draw_Players::bDrawBones)
	{
		ImGui::Indent();
		ImGui::SliderFloat("Thickness##Bones", &Draw_Players::fBonesThickness, 0.1f, 5.0f, "%.1f");
		ImGui::Checkbox("Show Bone Numbers", &Draw_Players::bBoneNumbers);
		ImGui::Unindent();
	}

	ImGui::Checkbox("Head Circle", &Draw_Players::bDrawHead);
	ImGui::Checkbox("Velocity Vector", &Draw_Players::bDrawVelocityVector);
	ImGui::Checkbox("Health Bars", &Draw_Players::bDrawHealthBar);

	ImGui::Spacing();
	ImGui::Checkbox("Show Level", &Draw_Players::bShowLevel);
	ImGui::Checkbox("Show Distance", &Draw_Players::bShowDistance);

	ImGui::Spacing();
	ImGui::SeparatorText("Souls Display");
	ImGui::Checkbox("Show Unsecured Souls", &Draw_Players::bDrawUnsecuredSouls);
	if (Draw_Players::bDrawUnsecuredSouls)
	{
		ImGui::Indent();
		ImGui::SetNextItemWidth(150.0f);
		ImGui::InputInt("Minimum Threshold", &Draw_Players::UnsecuredSoulsMinimumThreshold);
		ImGui::SetNextItemWidth(150.0f);
		ImGui::InputInt("Highlight Threshold", &Draw_Players::UnsecuredSoulsHighlightThreshold);
		ImGui::Unindent();
	}


	ImGui::Spacing();

	ImGui::SeparatorText("Troopers");
	ImGui::Checkbox("Enable Trooper ESP", &Draw_Troopers::bMasterToggle);
	ImGui::Checkbox("Hide Friendly Troopers", &Draw_Troopers::bHideFriendly);


	ImGui::Spacing();

	ImGui::SeparatorText("Map Objectives");
	ImGui::Checkbox("Jungle Camps", &Draw_Camps::bMasterToggle);
	ImGui::Checkbox("Sinner's Sacrifice (Vaults)", &Draw_Sinners::bMasterToggle);

	ImGui::End();
}