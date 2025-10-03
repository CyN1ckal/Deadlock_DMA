#include "pch.h"
#include "CBaseEntity.h"
#include "Deadlock/Entity List/EntityList.h"

bool CBaseEntity::IsFriendly() const
{
	return TeamNum == EntityList::m_PlayerControllers[Deadlock::m_LocalPlayerControllerAddress].TeamNum;
}

bool CBaseEntity::IsLocalPlayer(uintptr_t selfAddr) const {
    return selfAddr == Deadlock::m_LocalPlayerControllerAddress;
}

Vector3 CBaseEntity::GetLocalPlayerPosition(const CBaseEntity& pawn, uintptr_t addr)
{
    if (IsLocalPlayer(addr))
        return pawn.Position;

    return { 0.f,0.f,0.f };
}