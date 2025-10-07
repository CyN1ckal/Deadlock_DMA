#pragma once
#include "DMA/MultiScan.h"

namespace Offsets
{
	bool ResolveOffsets(DMA_Connection* Conn);
	bool ResolveGameEntitySystem(MultiScan& ms, DMA_Connection* Conn);
	bool ResolveLocalController(MultiScan& ms, DMA_Connection* Conn);
	bool ResolveViewMatrix(MultiScan& ms, DMA_Connection* Conn);
	bool ResolvePredictionPtr(MultiScan& ms, DMA_Connection* Conn);
	bool ResolveRotation(MultiScan& ms, DMA_Connection* Conn);

	inline constexpr std::ptrdiff_t FirstEntityList = 0x10;
	inline std::ptrdiff_t GameEntitySystem = 0;
	inline std::ptrdiff_t LocalController = 0;
	inline std::ptrdiff_t ViewMatrix = 0;
	inline std::ptrdiff_t PredictionPtr = 0;
	inline std::ptrdiff_t Rotation = 0;

	namespace Prediction
	{
		inline constexpr std::ptrdiff_t ServerTime = 0x68;
	}

	namespace SceneNode
	{
		inline constexpr std::ptrdiff_t Position = 0xD0;
		inline constexpr std::ptrdiff_t ModelState = 0x190;
		inline constexpr std::ptrdiff_t Dormant = 0x10B;
	}

	namespace ModelState
	{
		inline constexpr std::ptrdiff_t BoneArrayPtr = 0x80;
	}

	namespace Pawn
	{
		inline constexpr std::ptrdiff_t hController = 0x10F8;
	}

	namespace BaseEntity
	{
		inline constexpr std::ptrdiff_t GameSceneNode = 0x330;
		inline constexpr std::ptrdiff_t TeamNum = 0x3F3;
		inline constexpr std::ptrdiff_t CurrentHealth = 0x354;
	}

	namespace Controller
	{
		inline constexpr std::ptrdiff_t PlayerData = 0x8B8;
		inline constexpr std::ptrdiff_t m_hPawn = 0x6B4;

		namespace PlayerDataOffsets
		{
			inline constexpr std::ptrdiff_t Level = 0x8;
			inline constexpr std::ptrdiff_t MaxHealth = 0x10;
			inline constexpr std::ptrdiff_t HeroID = 0x1C;
			inline constexpr std::ptrdiff_t TotalSouls = 0x24;
			inline constexpr std::ptrdiff_t CurrentHealth = 0x4C;
		}
	}
}