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
	ZoneScoped;

	auto DrawList = ImGui::GetWindowDrawList();
	auto WindowPos = ImGui::GetWindowPos();

	ImGui::PushFont(nullptr, 16.0f);

	Draw_Players()();
	Draw_Troopers()();
	Draw_Camps()();
	Draw_Sinners()();

	ImGui::PopFont();
}

void ESP::RenderSettings()
{
	ImGui::Begin("ESP Settings");

	/*if (ImGui::CollapsingHeader("Master Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Indent();

		ImGui::Checkbox("Draw Troopers", &bDrawTroopers);
		ImGui::Checkbox("Draw Monster Camps", &bDrawCamps);
		ImGui::Checkbox("Draw Sinners", &bDrawSinners);
		ImGui::SeparatorText("Filters");
		ImGui::Checkbox("Hide Local Player", &bHideLocal);
		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader("Player Name Tags"))
	{
		ImGui::Indent();
		ImGui::Checkbox("Draw Player Name Tags", &NameTagSettings.bDrawNameTags);
		ImGui::Checkbox("Hide Friendly", &NameTagSettings.bHideFriendly);
		ImGui::Checkbox("Show Level", &NameTagSettings.bShowLevel);
		ImGui::Checkbox("Show Hero Name", &NameTagSettings.bShowHeroName);
		ImGui::Checkbox("Show Health", &NameTagSettings.bShowHealth);
		ImGui::Checkbox("Show Distance", &NameTagSettings.bShowDistance);
		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader("Player Bones"))
	{
		ImGui::Indent();
		ImGui::Checkbox("Draw Bones", &SkeletonSettings.bDrawSkeleton);
		ImGui::Checkbox("Hide Friendly Bones", &SkeletonSettings.bHideFriendly);
		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader("Debug"))
	{
		ImGui::Indent();
		ImGui::Checkbox("Bone Numbers", &bBoneNumbers);
		ImGui::Unindent();
	}*/

	ImGui::End();
}