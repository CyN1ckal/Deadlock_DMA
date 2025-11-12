#include "pch.h"

#include "Trooper List.h"

#include "Deadlock/Entity List/EntityList.h"

void TrooperList::Render()
{
	/* m_ControllerMutex is required for friendly check */
	std::scoped_lock Lock(EntityList::m_TrooperMutex, EntityList::m_ControllerMutex);

	static bool bHideFriendly = true;

	ImGui::Begin("Trooper List");

	ImGui::Checkbox("Hide Friendly", &bHideFriendly);

	for(auto& Trooper : EntityList::m_Troopers)
	{
		if (Trooper.IsInvalid()) continue;
		
		if (bHideFriendly && Trooper.IsFriendly()) continue;

		auto String = std::format("{0:X} {1:d}", Trooper.m_EntityAddress, Trooper.m_TeamNum);

		ImGui::Text(String.c_str());
	}

	ImGui::End();
}