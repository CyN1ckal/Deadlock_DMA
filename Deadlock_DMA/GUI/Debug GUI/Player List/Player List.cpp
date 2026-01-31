#include "pch.h"

#include "Player List.h"

#include "Deadlock/Entity List/EntityList.h"

void PlayerList::Render()
{
	if (!bSettings) return;

	ImGui::Begin("Controller List", &bSettings);

	std::scoped_lock lock(EntityList::m_PawnMutex, EntityList::m_ControllerMutex);

	if (ImGui::BeginTable("Players Table", 7))
	{
		ImGui::TableSetupColumn("Health");
		ImGui::TableSetupColumn("Hero ID");
		ImGui::TableSetupColumn("Hero Name");
		ImGui::TableSetupColumn("Distance");
		ImGui::TableSetupColumn("Souls");
		ImGui::TableSetupColumn("Pawn Address");
		ImGui::TableSetupColumn("Team ID");
		ImGui::TableHeadersRow();

		uint32_t PlayerNum = 0;

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
			ImGui::TableNextColumn();
			if (ImGui::Button(std::format("Copy Address##{}", PlayerNum).c_str()))
				ImGui::SetClipboardText(std::format("0x{:X}", Pawn.m_EntityAddress).c_str());
			ImGui::TableNextColumn();
			ImGui::Text("%d", Pawn.m_TeamNum);
			PlayerNum++;
		}

		ImGui::EndTable();
	}

	ImGui::End();
}