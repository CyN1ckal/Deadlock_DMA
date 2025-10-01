#include "pch.h"

#include "Player List.h"

#include "Deadlock/Entity List/EntityList.h"

void PlayerList::Render()
{
	std::scoped_lock lock(EntityList::PlayerPawnsMutex, EntityList::PlayerControllerMutex);

	ImGui::Begin("Controller List");

	for (auto& [address, pawn] : EntityList::m_PlayerPawns)
	{
		auto ControllerAddr = EntityList::GetEntityAddressFromHandle(pawn.hController);

		auto& Controller = EntityList::m_PlayerControllers[ControllerAddr];

		if (Controller.IsIncomplete() || pawn.IsIncomplete()) continue;

		ImGui::Text(std::format("{0:d} {1:d} {2:s}", Controller.m_CurrentHealth, Controller.m_MaxHealth, Controller.GetHeroName()).c_str());
	}

	ImGui::End();
}