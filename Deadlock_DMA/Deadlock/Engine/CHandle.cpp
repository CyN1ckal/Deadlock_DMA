#include "pch.h"

#include "CHandle.h"

#include "Deadlock/Entity List/EntityList.h"

size_t CHandle::GetEntityListIndex() const
{
	return (Data & 0x7FFF) / MAX_ENTITIES;
}

size_t CHandle::GetEntityEntryIndex() const
{
	return (Data & 0x7FFF) % MAX_ENTITIES;
}

bool CHandle::IsValid() const
{
	return (Data & 0x7FFF) <= (MAX_ENTITIES * MAX_ENTITY_LISTS);
}