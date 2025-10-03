#include "pch.h"

#include "Player List.h"

#include "Deadlock/Entity List/EntityList.h"

void PlayerList::Render()
{
	std::scoped_lock lock(EntityList::m_PawnMutex, EntityList::m_ControllerMutex);

	ImGui::Begin("Controller List");

	for (auto& [address, pawn] : EntityList::m_PlayerPawns)
	{
		auto ControllerAddr = EntityList::GetEntityAddressFromHandle(pawn.hController);

		auto& Controller = EntityList::m_PlayerControllers[ControllerAddr];

		if (Controller.IsIncomplete() || pawn.IsIncomplete()) continue;

		ImGui::Text(std::format("{0:d} {1:d} {2:s} {3:.0f}", Controller.CurrentHealth, Controller.m_MaxHealth, Controller.GetHeroName(), pawn.DistanceFromLocalPlayer()).c_str());
	}

	ImGui::End();
}