#include "pch.h"

#include "Player List.h"

#include "Deadlock/Entity List/EntityList.h"

void PlayerList::Render()
{
	std::scoped_lock lock(EntityList::m_PawnMutex, EntityList::m_ControllerMutex);

	ImGui::Begin("Controller List");

	for (auto& Pawn : EntityList::m_PlayerPawns)
	{
		uintptr_t ControllerAddr = EntityList::GetEntityAddressFromHandle(Pawn.m_hController);

		auto ControllerIt = std::find(EntityList::m_PlayerControllers.begin(), EntityList::m_PlayerControllers.end(), ControllerAddr);

		if (ControllerIt == EntityList::m_PlayerControllers.end()) continue;

		if (ControllerIt->IsInvalid() || Pawn.IsInvalid()) continue;

		ImGui::Text(std::format("{0:d} {1:d} {2:s} {3:.0f} {4:X}", ControllerIt->m_CurrentHealth, ControllerIt->m_MaxHealth, ControllerIt->GetHeroName(), Pawn.DistanceFromLocalPlayer(), ControllerAddr).c_str());
	}

	ImGui::End();
}