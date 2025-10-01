#pragma once
#include "CBaseEntity.h"
#include "Deadlock/Engine/CHandle.h"
#include "Deadlock/Const/HeroMap.hpp"
#include "Deadlock/Const/HeroEnum.hpp"

class CCitadelPlayerController : public CBaseEntity
{
public:
	CHandle m_hPawn{ 0 };
	int32_t m_CurrentLevel{ 0 };
	int32_t m_MaxHealth{ 0 };
	int32_t m_CurrentHealth{ 0 };
	HeroId m_HeroID{ 0 };

public:
	const bool IsDead() const { return m_CurrentHealth <= 0; }
	const std::string_view GetHeroName() const
	{
		auto it = HeroNames::HeroNameMap.find(m_HeroID);

		if (it != HeroNames::HeroNameMap.end())
			return it->second;

		return "Unknown";
	}

public:
	template<typename T>
	static void Read_1(VMMDLL_SCATTER_HANDLE vmsh, T& Controller, uintptr_t EntityAddress)
	{
		uintptr_t PawnHandleAddress = EntityAddress + Offsets::Controller::m_hPawn;
		VMMDLL_Scatter_PrepareEx(vmsh, PawnHandleAddress, sizeof(CHandle), reinterpret_cast<BYTE*>(&Controller.m_hPawn), nullptr);

		uintptr_t PlayerDataAddress = EntityAddress + Offsets::Controller::PlayerData;

		uintptr_t LevelAddress = PlayerDataAddress + Offsets::Controller::PlayerDataOffsets::Level;
		VMMDLL_Scatter_PrepareEx(vmsh, LevelAddress, sizeof(int32_t), reinterpret_cast<BYTE*>(&Controller.m_CurrentLevel), nullptr);

		uintptr_t MaxHealthAddress = PlayerDataAddress + Offsets::Controller::PlayerDataOffsets::MaxHealth;
		VMMDLL_Scatter_PrepareEx(vmsh, MaxHealthAddress, sizeof(int32_t), reinterpret_cast<BYTE*>(&Controller.m_MaxHealth), nullptr);

		uintptr_t CurrentHealthAddress = PlayerDataAddress + Offsets::Controller::PlayerDataOffsets::CurrentHealth;
		VMMDLL_Scatter_PrepareEx(vmsh, CurrentHealthAddress, sizeof(int32_t), reinterpret_cast<BYTE*>(&Controller.m_CurrentHealth), nullptr);

		uintptr_t HeroIDAddress = PlayerDataAddress + Offsets::Controller::PlayerDataOffsets::HeroID;
		VMMDLL_Scatter_PrepareEx(vmsh, HeroIDAddress, sizeof(int32_t), reinterpret_cast<BYTE*>(&Controller.m_HeroID), nullptr);
	}

private:

};