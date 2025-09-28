#pragma once
#include "Vector3.h"

class CBaseEntity
{
public:
	uintptr_t GameSceneNodeAddress{ 0 };
	int32_t Health{ 0 };
	Vector3 Position{ 0.0f };

public:
	template<typename T>
	static void Read_1(VMMDLL_SCATTER_HANDLE vmsh, T& Entity, uintptr_t EntityAddress)
	{
		uintptr_t GameSceneNodePtr = EntityAddress + Offsets::GameSceneNode;
		VMMDLL_Scatter_PrepareEx(vmsh, GameSceneNodePtr, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&Entity.GameSceneNodeAddress), nullptr);

		uintptr_t HealthPtr = EntityAddress + Offsets::Health;
		VMMDLL_Scatter_PrepareEx(vmsh, HealthPtr, sizeof(int32_t), reinterpret_cast<BYTE*>(&Entity.Health), nullptr);
	}

	template<typename T>
	static void Read_2(VMMDLL_SCATTER_HANDLE vmsh, T& Entity, uintptr_t EntityAddress)
	{
		uintptr_t PositionAddress = Entity.GameSceneNodeAddress + Offsets::Position;
		VMMDLL_Scatter_PrepareEx(vmsh, PositionAddress, sizeof(Vector3), reinterpret_cast<BYTE*>(&Entity.Position), nullptr);
	}

	template<typename T>
	static void QuickRead(VMMDLL_SCATTER_HANDLE vmsh, T& Entity, uintptr_t EntityAddress)
	{
		uintptr_t HealthPtr = EntityAddress + Offsets::Health;
		VMMDLL_Scatter_PrepareEx(vmsh, HealthPtr, sizeof(int32_t), reinterpret_cast<BYTE*>(&Entity.Health), nullptr);

		uintptr_t PositionAddress = Entity.GameSceneNodeAddress + Offsets::Position;
		VMMDLL_Scatter_PrepareEx(vmsh, PositionAddress, sizeof(Vector3), reinterpret_cast<BYTE*>(&Entity.Position), nullptr);
	}
};