#pragma once

#include "Deadlock/Classes/CCitadelPlayerController.h"

namespace Aimpoints
{
	enum class eAimpoint : uint8_t
	{
		eChest = 0,
		eHead = 1
	};

	/* Head, chest */
	using AimList = std::pair<size_t, size_t>;
	AimList GetAimpoints(HeroId ID);
}