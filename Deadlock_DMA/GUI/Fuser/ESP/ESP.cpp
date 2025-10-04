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

	if (bDrawCamps) RenderMonsterCamps();

	RenderSinners();

	ImGui::PopFont();
}

void ESP::RenderSettings()
{
	ImGui::Begin("ESP Settings");

	if (ImGui::CollapsingHeader("Master Settings", ImGuiTreeNodeFlags_DefaultOpen))
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
	}

	ImGui::End();
}

void ESP::RenderPlayers(const ImVec2 WindowPos, ImDrawList* DrawList)
{
	std::scoped_lock lock(EntityList::m_PawnMutex, EntityList::m_ControllerMutex);

	for (auto& [Addr, Pawn] : EntityList::m_PlayerPawns)
	{
		if (Pawn.IsIncomplete()) continue;

		auto AssociatedControllerAddr = EntityList::GetEntityAddressFromHandle(Pawn.hController);

		if (bHideLocal && Pawn.IsLocalPlayer(AssociatedControllerAddr)) continue;

		auto& Controller = EntityList::m_PlayerControllers[AssociatedControllerAddr];

		if (Controller.IsIncomplete()) continue;

		if (Controller.IsDead()) continue;

		if (NameTagSettings.bDrawNameTags) Pawn.DrawNameTag(WindowPos, DrawList, Controller);

		if (bBoneNumbers) Pawn.DrawBoneNumbers();

		if (SkeletonSettings.bDrawSkeleton) Pawn.DrawSkeleton(WindowPos, DrawList);
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

void ESP::RenderMonsterCamps()
{
	std::scoped_lock Lock(EntityList::m_MonsterCampMutex);

	ImGui::PushStyleColor(ImGuiCol_Text, ColorPicker::MonsterCampColor);

	for (auto& [Addr, Camp] : EntityList::m_MonsterCamps)
	{
		if (Camp.IsIncomplete()) continue;

		if (Camp.IsDormant()) continue;

		if (Camp.CurrentHealth < 1) continue;

		Vector2 ScreenPos{};
		if (!Deadlock::WorldToScreen(Camp.Position, ScreenPos)) continue;

		std::string CampString = std::format("[{}]", Camp.CurrentHealth);

		auto TextSize = ImGui::CalcTextSize(CampString.c_str());

		ImGui::SetCursorPos({ ScreenPos.x - (TextSize.x / 2.0f), ScreenPos.y });

		ImGui::Text(CampString.c_str());
	}

	ImGui::PopStyleColor(1);
}

void ESP::RenderSinners()
{
	std::scoped_lock Lock(EntityList::m_SinnerMutex);

	ImGui::PushStyleColor(ImGuiCol_Text, ColorPicker::SinnersColor);

	for (auto& [Addr, Sinner] : EntityList::m_Sinners)
	{
		if (Sinner.IsIncomplete()) continue;

		if (Sinner.IsDormant()) continue;

		if (Sinner.CurrentHealth < 1) continue;

		Vector2 ScreenPos{};
		if (!Deadlock::WorldToScreen(Sinner.Position, ScreenPos)) continue;

		static const std::string SinnerString = "SS";

		auto TextSize = ImGui::CalcTextSize(SinnerString.c_str());

		ImGui::SetCursorPos({ ScreenPos.x - (TextSize.x / 2.0f), ScreenPos.y });

		ImGui::Text(SinnerString.c_str());
	}

	ImGui::PopStyleColor(1);
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