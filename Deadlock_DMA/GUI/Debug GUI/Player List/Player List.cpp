#include "pch.h"

#include "Player List.h"

#include "Deadlock/Entity List/EntityList.h"

void PlayerList::Render()
{
	std::scoped_lock lock(EntityList::m_PawnMutex, EntityList::m_ControllerMutex);

	ImGui::Begin("Controller List");

	if (ImGui::BeginTable("Players Table", 5))
	{
		ImGui::TableSetupColumn("Health");
		ImGui::TableSetupColumn("Hero ID");
		ImGui::TableSetupColumn("Hero Name");
		ImGui::TableSetupColumn("Distance");
		ImGui::TableSetupColumn("Souls");
		ImGui::TableHeadersRow();

		for (auto& Pawn : EntityList::m_PlayerPawns)
		{
			uintptr_t ControllerAddr = EntityList::GetEntityAddressFromHandle(Pawn.m_hController);

			auto ControllerIt = std::find(EntityList::m_PlayerControllers.begin(), EntityList::m_PlayerControllers.end(), ControllerAddr);

			if (ControllerIt == EntityList::m_PlayerControllers.end()) continue;

			if (ControllerIt->IsInvalid() || Pawn.IsInvalid()) continue;

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("%d / %d", ControllerIt->m_CurrentHealth, ControllerIt->m_MaxHealth);
			ImGui::TableNextColumn();
			ImGui::Text("%d", static_cast<int>(ControllerIt->m_HeroID));
			ImGui::TableNextColumn();
			ImGui::Text(ControllerIt->GetHeroName().data());
			ImGui::TableNextColumn();
			ImGui::Text("%.2f m", Pawn.DistanceFromLocalPlayer(true));
			ImGui::TableNextColumn();
			ImGui::Text("%d", ControllerIt->m_TotalSouls);
		}

		ImGui::EndTable();
	}

	ImGui::End();
}