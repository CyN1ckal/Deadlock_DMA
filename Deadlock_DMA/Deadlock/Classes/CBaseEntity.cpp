#include "pch.h"
#include "CBaseEntity.h"
#include "Deadlock/Entity List/EntityList.h"

const bool CBaseEntity::IsFriendly() const
{
	return TeamNum == EntityList::m_PlayerControllers[Deadlock::m_LocalPlayerControllerAddress].TeamNum;
}

const bool CBaseEntity::IsLocalPlayer(uintptr_t EntityAddress) const
{
	return EntityAddress == Deadlock::m_LocalPlayerControllerAddress;
}

const float CBaseEntity::DistanceFromLocalPlayer() const
{
	auto LocalPlayer = EntityList::m_PlayerPawns.find(Deadlock::m_LocalPlayerPawnAddress);

	if (LocalPlayer == EntityList::m_PlayerPawns.end())
		return 0.0f;

	return Position.Distance(LocalPlayer->second.Position);
}