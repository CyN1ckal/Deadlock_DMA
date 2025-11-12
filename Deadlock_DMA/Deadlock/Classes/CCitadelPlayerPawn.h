#pragma once
#include "pch.h"
#include "Deadlock/Engine/CHandle.h"
#include "CBaseEntity.h"
#include "CCitadelPlayerController.h"

constexpr int MAX_BONES = 70;

class CCitadelPlayerPawn : public CBaseEntity
{
public:
	Vector3 m_BonePositions[MAX_BONES]{ 0.0f };
	uintptr_t m_BoneArrayAddress{ 0 };
	CHandle m_hController{ 0 };

public:
	void DrawSkeleton(const ImVec2& WindowPos, ImDrawList* DrawList) const;
	void DrawNameTag(const ImVec2& WindowPos, ImDrawList* DrawList, CCitadelPlayerController& AssociatedController) const;
	void DrawBoneNumbers() const;

private:
	void PrepareBoneRead(VMMDLL_SCATTER_HANDLE vmsh)
	{
		for (int i = 0; i < MAX_BONES; i++)
		{
			uintptr_t BoneAddress = m_BoneArrayAddress + (i * 0x20);
			VMMDLL_Scatter_PrepareEx(vmsh, BoneAddress, sizeof(Vector3), reinterpret_cast<BYTE*>(&m_BonePositions[i]), nullptr);
		}
	}

public:
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
	{
		CBaseEntity::PrepareRead_1(vmsh, false);

		uintptr_t hControllerPtr = m_EntityAddress + Offsets::Pawn::hController;
		VMMDLL_Scatter_PrepareEx(vmsh, hControllerPtr, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_hController.Data), nullptr);
	}

	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
	{
		if (!m_GameSceneNodeAddress)
			SetInvalid();

		CBaseEntity::PrepareRead_2(vmsh);

		if (IsInvalid())
			return;

		uintptr_t BoneArrayPtrAddress = m_GameSceneNodeAddress + Offsets::SceneNode::ModelState + Offsets::ModelState::BoneArrayPtr;
		VMMDLL_Scatter_PrepareEx(vmsh, BoneArrayPtrAddress, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_BoneArrayAddress), nullptr);
	}

	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
	{
		if (IsInvalid())
			return;

		PrepareBoneRead(vmsh);
	}
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
	{
		if (IsInvalid())
			return;

		CBaseEntity::QuickRead(vmsh, false);

		PrepareBoneRead(vmsh);
	}
};