#pragma once
#include "pch.h"
#include "Deadlock/Entity List/EntityList.h"

class CHandle
{
public:
	uint32_t Data{ 0 };

public:
	size_t GetEntityListIndex() const { return (Data & 0x7FFF) / MAX_ENTITIES; }
	size_t GetEntityEntryIndex() const { return (Data & 0x7FFF) % MAX_ENTITIES; }
	bool IsIncomplete() const { return (Data & 0x7FFF) <= (MAX_ENTITIES * MAX_ENTITY_LISTS); }
};