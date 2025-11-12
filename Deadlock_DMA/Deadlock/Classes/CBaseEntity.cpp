#include "pch.h"
#include "CBaseEntity.h"
#include "Deadlock/Entity List/EntityList.h"

const bool CBaseEntity::IsFriendly() const
{
	std::scoped_lock lock(Deadlock::m_LocalAddressMutex);

	auto LocalControllerIt = std::find(EntityList::m_PlayerControllers.begin(), EntityList::m_PlayerControllers.end(), Deadlock::m_LocalPlayerControllerAddress);

	if (LocalControllerIt == EntityList::m_PlayerControllers.end())
		return false;

	return m_TeamNum == LocalControllerIt->m_TeamNum;
}

const bool CBaseEntity::IsLocalPlayer() const
{
	std::scoped_lock lock(Deadlock::m_LocalAddressMutex);
	return m_EntityAddress == Deadlock::m_LocalPlayerControllerAddress || m_EntityAddress == Deadlock::m_LocalPlayerPawnAddress;
}

constexpr uint32_t HammerUnitsPerMeter = 52;
const float CBaseEntity::DistanceFromLocalPlayer(bool bInMeters) const
{
	auto LocalPawnIt = std::find(std::begin(EntityList::m_PlayerPawns), std::end(EntityList::m_PlayerPawns), Deadlock::m_LocalPlayerPawnAddress);

	if (LocalPawnIt == EntityList::m_PlayerPawns.end())
		return 0.0f;

	auto Distance = m_Position.Distance(LocalPawnIt->m_Position);

	if (bInMeters)
		return Distance / HammerUnitsPerMeter; // Convert cm to m

	return Distance;
}