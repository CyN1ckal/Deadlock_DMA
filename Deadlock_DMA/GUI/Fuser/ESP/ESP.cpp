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
		Draw_Players::operator()();

	if (Draw_Troopers::bMasterToggle)
		Draw_Troopers::operator()();

	if (Draw_Camps::bMasterToggle)
		Draw_Camps::operator()();

	if (Draw_Sinners::bMasterToggle)
		Draw_Sinners::operator()();

	ImGui::PopFont();
}

void ESP::RenderSettings()
{
	ImGui::Begin("ESP Settings");

	ImGui::Checkbox("Enable ESP", &bMasterToggle);

	if (ImGui::CollapsingHeader("Players", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Indent();
		ImGui::Checkbox("Master Toggle", &Draw_Players::bMasterToggle);
		ImGui::Checkbox("Hide Friendly", &Draw_Players::bHideFriendly);

		ImGui::Checkbox("Bones", &Draw_Players::bDrawBones);
		if (Draw_Players::bDrawBones)
		{
			ImGui::Indent();
			ImGui::SliderFloat("Bones Thickness", &Draw_Players::fBonesThickness, 0.1f, 5.0f, "%.1f");
			ImGui::Unindent();
		};

		ImGui::Checkbox("Head Circle", &Draw_Players::bDrawHead);
		ImGui::Checkbox("Velocity Vector", &Draw_Players::bDrawVelocityVector);
		ImGui::Checkbox("Health Bars", &Draw_Players::bDrawHealthBar);
		ImGui::Checkbox("Unsecured Souls", &Draw_Players::bDrawUnsecuredSouls);
		ImGui::Indent();
		ImGui::SetNextItemWidth(50.0f);
		ImGui::InputScalarN("Minimum Threshold", ImGuiDataType_S32, &Draw_Players::UnsecuredSoulsMinimumThreshold, 1);
		ImGui::SetNextItemWidth(50.0f);
		ImGui::InputScalarN("Highlight Threshold", ImGuiDataType_S32, &Draw_Players::UnsecuredSoulsHighlightThreshold, 1);
		ImGui::Unindent();
		ImGui::Checkbox("Hide Local", &Draw_Players::bHideLocalPlayer);
		ImGui::Checkbox("Show Distance", &Draw_Players::bShowDistance);
		ImGui::Checkbox("Bone Numbers", &Draw_Players::bBoneNumbers);
		ImGui::Unindent();
	}

	ImGui::Checkbox("Draw Troopers", &Draw_Troopers::bMasterToggle);
	ImGui::Checkbox("Hide Friendly Troopers", &Draw_Troopers::bHideFriendly);

	ImGui::Checkbox("Draw Monster Camps", &Draw_Camps::bMasterToggle);

	ImGui::Checkbox("Draw Sinners", &Draw_Sinners::bMasterToggle);

	ImGui::End();
}