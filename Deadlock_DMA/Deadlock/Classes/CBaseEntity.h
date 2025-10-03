#pragma once
#include "Deadlock/Engine/Vector3.h"
#
class CBaseEntity
{
public:
	Vector3 Position{ 0.0f };
	uintptr_t GameSceneNodeAddress{ 0 };
	uint32_t Flags{ 0 };
	uint8_t TeamNum{ 0 };

public:
	const bool IsIncomplete() const { return Flags & 0x1; }
	void SetInvalid() { Flags |= 0x1; }
	const bool IsFriendly() const;
	const bool IsLocalPlayer(uintptr_t EntityAddress) const;
	const float DistanceFromLocalPlayer() const;

public:
	template<typename T>
	static void Read_1(VMMDLL_SCATTER_HANDLE vmsh, T& Entity, uintptr_t EntityAddress)
	{
		uintptr_t GameSceneNodePtr = EntityAddress + Offsets::BaseEntity::GameSceneNode;
		VMMDLL_Scatter_PrepareEx(vmsh, GameSceneNodePtr, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&Entity.GameSceneNodeAddress), nullptr);

		uintptr_t TeamNumPtr = EntityAddress + Offsets::BaseEntity::TeamNum;
		VMMDLL_Scatter_PrepareEx(vmsh, TeamNumPtr, sizeof(uint8_t), reinterpret_cast<BYTE*>(&Entity.TeamNum), nullptr);
	}

	template<typename T>
	static void Read_2(VMMDLL_SCATTER_HANDLE vmsh, T& Entity, uintptr_t EntityAddress)
	{
		if (!Entity.GameSceneNodeAddress)
		{
			Entity.SetInvalid();
			return;
		}

		uintptr_t PositionAddress = Entity.GameSceneNodeAddress + Offsets::SceneNode::Position;
		VMMDLL_Scatter_PrepareEx(vmsh, PositionAddress, sizeof(Vector3), reinterpret_cast<BYTE*>(&Entity.Position), nullptr);
	}
};