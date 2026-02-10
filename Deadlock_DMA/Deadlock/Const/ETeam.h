#pragma once
#include <cstdint>

enum class ETeam : uint8_t
{
	HIDDEN_KING = 2,
	ARCH_MOTHER = 3,
	UNKNOWN = std::numeric_limits<uint8_t>::max()
};