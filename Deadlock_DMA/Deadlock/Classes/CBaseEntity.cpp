#include "pch.h"
#include "CBaseEntity.h"
#include "Deadlock/Entity List/EntityList.h"

bool CBaseEntity::IsFriendly() const
{
	return TeamNum == EntityList::m_PlayerControllers[Deadlock::m_LocalPlayerControllerAddress].TeamNum;
}
