#pragma once

namespace Offsets
{
	inline constexpr std::ptrdiff_t GameEntitySystem = 0x26E4838;
	inline constexpr std::ptrdiff_t EntityList = 0x10;
	inline constexpr std::ptrdiff_t ViewMatrix = 0x2B4FC60;

	inline constexpr std::ptrdiff_t GameSceneNode = 0x330;
	inline constexpr std::ptrdiff_t Health = 0x354;

	inline constexpr std::ptrdiff_t m_hPawn = 0x6B4;
	inline constexpr std::ptrdiff_t hController = 0x10F8;

	inline constexpr std::ptrdiff_t Position = 0xD0;

	namespace BaseEntity
	{
		inline constexpr std::ptrdiff_t TeamNum = 0x3F3;
	}

	namespace Controller
	{
		inline constexpr std::ptrdiff_t PlayerData = 0x8B8;

		namespace PlayerDataOffsets
		{
			inline constexpr std::ptrdiff_t Level = 0x8;
			inline constexpr std::ptrdiff_t CurrentHealth = 0x4C;
			inline constexpr std::ptrdiff_t MaxHealth = 0x10;
			inline constexpr std::ptrdiff_t HeroID = 0x1C;
		}
	}
}