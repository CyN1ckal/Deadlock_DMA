#pragma once
#include "pch.h"
#include "Deadlock/Engine/CHandle.h"
#include "CBaseEntity.h"
#include "CCitadelPlayerController.h"

constexpr int MAX_BONES = 70;

class CCitadelPlayerPawn : public CBaseEntity
{
public:
	Vector3 BonePositions[MAX_BONES]{ 0.0f };
	uintptr_t BoneArrayAddress{ 0 };
	CHandle hController{ 0 };

public:
	void DrawSkeleton(const ImVec2& WindowPos, ImDrawList* DrawList) const;
	void DrawNameTag(const ImVec2& WindowPos, ImDrawList* DrawList, CCitadelPlayerController& AssociatedController);

private:
	static void PrepareBoneRead(VMMDLL_SCATTER_HANDLE vmsh, uintptr_t BoneArrayAddress, BYTE* OutPosition, uint32_t BoneIndex)
	{
		uintptr_t BoneAddress = BoneArrayAddress + (BoneIndex * 0x20);
		VMMDLL_Scatter_PrepareEx(vmsh, BoneAddress, sizeof(Vector3), OutPosition, nullptr);
	}

public:
	template<typename T>
	static void Read_1(VMMDLL_SCATTER_HANDLE vmsh, T& PlayerPawn, uintptr_t PlayerPawnAddress)
	{
		uintptr_t hControllerPtr = PlayerPawnAddress + Offsets::Pawn::hController;
		VMMDLL_Scatter_PrepareEx(vmsh, hControllerPtr, sizeof(uint32_t), reinterpret_cast<BYTE*>(&PlayerPawn.hController.Data), nullptr);
	}

	template <typename T>
	static void Read_2(VMMDLL_SCATTER_HANDLE vmsh, T& PlayerPawn, uintptr_t PlayerPawnAddress)
	{
		if (!PlayerPawn.GameSceneNodeAddress)
		{
			PlayerPawn.SetInvalid();
			return;
		}

		uintptr_t BoneArrayPtrAddress = PlayerPawn.GameSceneNodeAddress + Offsets::SceneNode::ModelState + Offsets::ModelState::BoneArrayPtr;
		VMMDLL_Scatter_PrepareEx(vmsh, BoneArrayPtrAddress, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&PlayerPawn.BoneArrayAddress), nullptr);
	}

	template <typename T>
	static void Read_3(VMMDLL_SCATTER_HANDLE vmsh, T& PlayerPawn, uintptr_t PlayerPawnAddress)
	{
		for (int i = 0; i < MAX_BONES; i++)
			PrepareBoneRead(vmsh, PlayerPawn.BoneArrayAddress, reinterpret_cast<BYTE*>(&PlayerPawn.BonePositions[i]), i);
	}
};