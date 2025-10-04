#pragma once
#include "Deadlock/Engine/Vector3.h"
#
class CBaseEntity
{
public:
	Vector3 Position{ 0.0f };
	uintptr_t GameSceneNodeAddress{ 0 };
	int32_t CurrentHealth{ 0 };
	uint8_t Flags{ 0 };
	uint8_t TeamNum{ 0 };
	uint8_t Dormant{ 0 };

public:
	const bool IsIncomplete() const { return Flags & 0x1; }
	void SetInvalid() { Flags |= 0x1; }
	const bool IsFriendly() const;
	const bool IsLocalPlayer(uintptr_t EntityAddress) const;
	const float DistanceFromLocalPlayer(bool bInMeters = 0) const;
	const bool IsDormant() const { return Dormant; }

public:
	template<typename T>
	static void Read_1(VMMDLL_SCATTER_HANDLE vmsh, T& Entity, uintptr_t EntityAddress, bool bReadHealth = 0)
	{
		uintptr_t GameSceneNodePtr = EntityAddress + Offsets::BaseEntity::GameSceneNode;
		VMMDLL_Scatter_PrepareEx(vmsh, GameSceneNodePtr, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&Entity.GameSceneNodeAddress), nullptr);

		uintptr_t TeamNumPtr = EntityAddress + Offsets::BaseEntity::TeamNum;
		VMMDLL_Scatter_PrepareEx(vmsh, TeamNumPtr, sizeof(uint8_t), reinterpret_cast<BYTE*>(&Entity.TeamNum), nullptr);

		if (bReadHealth)
		{
			uintptr_t CurrentHealthPtr = EntityAddress + Offsets::BaseEntity::CurrentHealth;
			VMMDLL_Scatter_PrepareEx(vmsh, CurrentHealthPtr, sizeof(uint32_t), reinterpret_cast<BYTE*>(&Entity.CurrentHealth), nullptr);
		}
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

		uintptr_t DormantAddress = Entity.GameSceneNodeAddress + Offsets::SceneNode::Dormant;
		VMMDLL_Scatter_PrepareEx(vmsh, DormantAddress, sizeof(uint8_t), reinterpret_cast<BYTE*>(&Entity.Dormant), nullptr);
	}
};