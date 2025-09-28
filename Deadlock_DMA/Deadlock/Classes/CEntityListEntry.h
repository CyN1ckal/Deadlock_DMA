#pragma once
#include "pch.h"	

class CEntityListEntry
{
public:
	uintptr_t pEntity; //0x0000
	char pad_0008[24]; //0x0008
	uintptr_t NamePtr; //0x0020
	char pad_0028[80]; //0x0028
}; //Size: 0x0078
static_assert(sizeof(CEntityListEntry) == 0x78);