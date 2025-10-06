#include "pch.h"
#include "CBaseEntity.h"
#include "Deadlock/Entity List/EntityList.h"

const bool CBaseEntity::IsFriendly() const
{
	std::scoped_lock lock(Deadlock::m_LocalAddressMutex);
	return TeamNum == EntityList::m_PlayerControllers[Deadlock::m_LocalPlayerControllerAddress].TeamNum;
}

const bool CBaseEntity::IsLocalPlayer(uintptr_t EntityAddress) const
{
	std::scoped_lock lock(Deadlock::m_LocalAddressMutex);
	return EntityAddress == Deadlock::m_LocalPlayerControllerAddress || EntityAddress == Deadlock::m_LocalPlayerPawnAddress;
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