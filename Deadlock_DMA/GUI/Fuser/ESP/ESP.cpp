#include "pch.h"

#include "ESP.h"

#include "Deadlock/Entity List/EntityList.h"

void ESP::OnFrame()
{
	std::scoped_lock lock(EntityList::PlayerPawnsMutex, EntityList::PlayerControllerMutex);

	auto DrawList = ImGui::GetWindowDrawList();
	auto WindowPos = ImGui::GetWindowPos();

	ImGui::PushFont(nullptr, 16.0f);

	for (auto& [Addr, Pawn] : EntityList::m_PlayerPawns)
	{
		auto AssociatedControllerAddr = EntityList::GetEntityAddressFromHandle(Pawn.hController);

		if (bHideLocal && Pawn.IsLocalPlayer(AssociatedControllerAddr)) continue;

		auto& Controller = EntityList::m_PlayerControllers[AssociatedControllerAddr];

		if (Controller.IsDead()) continue;
		if (bHideFriendly && Controller.IsFriendly()) continue;

		if (NameTagSettings.bDrawNameTags) Pawn.DrawNameTag(WindowPos, DrawList, Controller);

		if (bBoneNumbers) Pawn.DrawBoneNumbers();

		if (bDrawBones) Pawn.DrawSkeleton(WindowPos, DrawList);
	}

	ImGui::PopFont();
}

void ESP::RenderSettings()
{
	ImGui::Begin("ESP Settings");

	if (ImGui::CollapsingHeader("Master Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Hide Friendly", &bHideFriendly);

		ImGui::Checkbox("Draw Bones", &bDrawBones);

		ImGui::Checkbox("Hide Local Player", &bHideLocal);
	}

	if (ImGui::CollapsingHeader("Name Tags"))
	{
		ImGui::Checkbox("Draw Name Tags", &NameTagSettings.bDrawNameTags);
		ImGui::Checkbox("Show Distance", &NameTagSettings.bShowDistance);
		ImGui::Checkbox("Show Level", &NameTagSettings.bShowLevel);
		ImGui::Checkbox("Show Hero Name", &NameTagSettings.bShowHeroName);
	}

	if (ImGui::CollapsingHeader("Debug"))
		ImGui::Checkbox("Bone Numbers", &bBoneNumbers);

	ImGui::End();
}