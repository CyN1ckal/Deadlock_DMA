#include "pch.h"

#include "ESP.h"

#include "Deadlock/Entity List/EntityList.h"

#include "GUI/Color Picker/Color Picker.h"

void ESP::OnFrame()
{
	auto DrawList = ImGui::GetWindowDrawList();
	auto WindowPos = ImGui::GetWindowPos();

	ImGui::PushFont(nullptr, 16.0f);

	RenderPlayers(WindowPos, DrawList);

	if (bDrawTroopers) RenderTroopers();

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

		ImGui::Checkbox("Draw Troopers", &bDrawTroopers);
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

void ESP::RenderPlayers(const ImVec2 WindowPos, ImDrawList* DrawList)
{
	std::scoped_lock lock(EntityList::m_PawnMutex, EntityList::m_ControllerMutex);

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
}

void ESP::RenderTroopers()
{
	/* m_ControllerMutex is required for friendly check */
	std::scoped_lock Lock(EntityList::m_TrooperMutex, EntityList::m_ControllerMutex);

	for (auto& [Addr, Trooper] : EntityList::m_Troopers)
	{
		if (Trooper.IsIncomplete()) continue;

		if (Trooper.IsDormant()) continue;

		if (Trooper.CurrentHealth < 1) continue;

		DrawTrooper(Trooper);
	}
}

void ESP::DrawTrooper(CBaseEntity& Trooper)
{
	Vector2 ScreenPos{};
	if (!Deadlock::WorldToScreen(Trooper.Position, ScreenPos)) return;

	auto HealthString = std::to_string(Trooper.CurrentHealth);
	auto TextSize = ImGui::CalcTextSize(HealthString.c_str());

	ImGui::PushStyleColor(ImGuiCol_Text, (Trooper.IsFriendly()) ? ColorPicker::FriendlyNameTagColor : ColorPicker::EnemyNameTagColor);

	ImGui::SetCursorPos({ ScreenPos.x - (TextSize.x / 2.0f), ScreenPos.y });

	ImGui::Text(HealthString.c_str());

	ImGui::PopStyleColor();
}