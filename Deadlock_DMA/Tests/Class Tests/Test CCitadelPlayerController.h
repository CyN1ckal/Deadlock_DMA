#pragma once
#include "Deadlock/Classes/CCitadelPlayerController.h"

TEST_CASE("CCitadelPlayerController::Missing m_EntityAddress")
{
	CCitadelPlayerController Entity(INVALID_ADDRESS);
	Entity.PrepareRead_1(TEST_VMSH);
	REQUIRE(Entity.IsInvalid() == true);
}

TEST_CASE("CCitadelPlayerController::Valid m_EntityAddress")
{
	CCitadelPlayerController Entity(VALID_ADDRESS);
	Entity.PrepareRead_1(TEST_VMSH);
	REQUIRE(Entity.IsInvalid() == false);
}