#pragma once

namespace Offsets
{
	// 48 8B 0D ? ? ? ? 8B FD //
	inline constexpr std::ptrdiff_t GameEntitySystem = 0x26EC5D8;
	inline constexpr std::ptrdiff_t FirstEntityList = 0x10;

	// 48 3B 35 //
	inline constexpr std::ptrdiff_t LocalController = 0x2B3E4B8;
	
	// 49 8D 87 ? ? ? ? 48 C1 E2 //
	inline constexpr std::ptrdiff_t ViewMatrix = 0x2B5C5E0;

	// 48 8B 05 ? ? ? ? 38 58 //
	inline constexpr std::ptrdiff_t PredictionPtr = 0x2495708;

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