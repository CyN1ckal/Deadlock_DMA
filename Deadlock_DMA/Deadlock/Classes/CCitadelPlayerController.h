#pragma once
#include "CBaseEntity.h"

class CCitadelPlayerController : public CBaseEntity
{
public:
	uint32_t hPawn = 0;

public:
	template<typename T>
	static void Read_1(VMMDLL_SCATTER_HANDLE vmsh, T& Controller, uintptr_t EntityAddress)
	{
		uintptr_t PawnHandleAddress = EntityAddress + Offsets::hPawn;
		VMMDLL_Scatter_PrepareEx(vmsh, PawnHandleAddress, sizeof(uint32_t), reinterpret_cast<BYTE*>(&Controller.hPawn), nullptr);
	}

	uint32_t GetPawnEntityIndex() const
	{
		return hPawn & 0x7FFF;
	}

private:

};