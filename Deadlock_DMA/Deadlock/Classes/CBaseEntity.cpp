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

constexpr uint32_t HammerUnitsPerMeter = 52;
const float CBaseEntity::DistanceFromLocalPlayer(bool bInMeters) const
{
	auto LocalPlayer = EntityList::m_PlayerPawns.find(Deadlock::m_LocalPlayerPawnAddress);

	if (LocalPlayer == EntityList::m_PlayerPawns.end())
		return 0.0f;

	auto Distance = Position.Distance(LocalPlayer->second.Position);

	if (bInMeters)
		return Distance / HammerUnitsPerMeter; // Convert cm to m

	return Distance;
}