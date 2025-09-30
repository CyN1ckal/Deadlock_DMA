#pragma once
#include "pch.h"
#include "CBaseEntity.h"

class CCitadelPlayerPawn : public CBaseEntity
{
public:
	CHandle hController{ 0 };

public:
	template<typename T>
	static void Read(VMMDLL_SCATTER_HANDLE vmsh, T& PlayerPawn, uintptr_t PlayerPawnAddress)
	{
		uintptr_t hControllerPtr = PlayerPawnAddress + Offsets::Pawn::hController;
		VMMDLL_Scatter_PrepareEx(vmsh, hControllerPtr, sizeof(uint32_t), reinterpret_cast<BYTE*>(&PlayerPawn.hController.Data), nullptr);
	}
};