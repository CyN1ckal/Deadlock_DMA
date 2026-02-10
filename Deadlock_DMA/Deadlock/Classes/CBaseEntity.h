#pragma once
#include "Deadlock/Engine/Vector3.h"
#include "Deadlock/Const/ETeam.h"

class CBaseEntity
{
public:
	uintptr_t m_EntityAddress{ 0 };
	Vector3 m_Position{ 0.0f };
	uintptr_t m_GameSceneNodeAddress{ 0 };
	int32_t m_CurrentHealth{ 0 };
	uint8_t m_Flags{ 0 };
	ETeam m_TeamNum{ 0 };
	uint8_t m_Dormant{ 0 };

public:
	const bool IsInvalid() const { return m_Flags & 0x1; }
	void SetInvalid() { m_Flags |= 0x1; }
	const bool IsFriendly() const;
	const bool IsLocalPlayer() const;
	const float DistanceFromLocalPlayer(bool bInMeters = 0) const;
	const bool IsDormant() const { return m_Dormant; }

	CBaseEntity(uintptr_t _EntityAddress) : m_EntityAddress(_EntityAddress) {};

public:
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh, bool bReadHealth = true)
	{
		if (!m_EntityAddress)
			SetInvalid();

		if (IsInvalid())
			return;

		uintptr_t GameSceneNodePtr = m_EntityAddress + Offsets::BaseEntity::GameSceneNode;
		VMMDLL_Scatter_PrepareEx(vmsh, GameSceneNodePtr, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_GameSceneNodeAddress), nullptr);

		uintptr_t TeamNumPtr = m_EntityAddress + Offsets::BaseEntity::TeamNum;
		VMMDLL_Scatter_PrepareEx(vmsh, TeamNumPtr, sizeof(uint8_t), reinterpret_cast<BYTE*>(&m_TeamNum), nullptr);

		if (bReadHealth)
		{
			uintptr_t CurrentHealthPtr = m_EntityAddress + Offsets::BaseEntity::CurrentHealth;
			VMMDLL_Scatter_PrepareEx(vmsh, CurrentHealthPtr, sizeof(int32_t), reinterpret_cast<BYTE*>(&m_CurrentHealth), nullptr);
		}
	}

	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
	{
		if (!m_GameSceneNodeAddress)
			SetInvalid();

		if (IsInvalid())
			return;

		uintptr_t PositionAddress = m_GameSceneNodeAddress + Offsets::SceneNode::Position;
		VMMDLL_Scatter_PrepareEx(vmsh, PositionAddress, sizeof(Vector3), reinterpret_cast<BYTE*>(&m_Position), nullptr);

		uintptr_t DormantAddress = m_GameSceneNodeAddress + Offsets::SceneNode::Dormant;
		VMMDLL_Scatter_PrepareEx(vmsh, DormantAddress, sizeof(uint8_t), reinterpret_cast<BYTE*>(&m_Dormant), nullptr);
	}

	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh, bool bReadHealth = true)
	{
		if (IsInvalid())
			return;

		if (bReadHealth)
		{
			uintptr_t CurrentHealthPtr = m_EntityAddress + Offsets::BaseEntity::CurrentHealth;
			VMMDLL_Scatter_PrepareEx(vmsh, CurrentHealthPtr, sizeof(int32_t), reinterpret_cast<BYTE*>(&m_CurrentHealth), nullptr);
		}

		uintptr_t PositionAddress = m_GameSceneNodeAddress + Offsets::SceneNode::Position;
		VMMDLL_Scatter_PrepareEx(vmsh, PositionAddress, sizeof(Vector3), reinterpret_cast<BYTE*>(&m_Position), nullptr);

		uintptr_t DormantAddress = m_GameSceneNodeAddress + Offsets::SceneNode::Dormant;
		VMMDLL_Scatter_PrepareEx(vmsh, DormantAddress, sizeof(uint8_t), reinterpret_cast<BYTE*>(&m_Dormant), nullptr);
	}

	bool operator==(const CBaseEntity& Other) const
	{
		return m_EntityAddress == Other.m_EntityAddress;
	}
};