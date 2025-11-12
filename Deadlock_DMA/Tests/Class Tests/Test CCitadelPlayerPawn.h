#pragma once

TEST_CASE("CCitadelPlayerPawn::Missing m_EntityAddress")
{
	CCitadelPlayerPawn Entity(INVALID_ADDRESS);
	Entity.PrepareRead_1(TEST_VMSH);
	REQUIRE(Entity.IsInvalid() == true);
}

TEST_CASE("CCitadelPlayerPawn::Valid m_EntityAddress")
{
	CCitadelPlayerPawn Entity(VALID_ADDRESS);
	Entity.PrepareRead_1(TEST_VMSH);
	REQUIRE(Entity.IsInvalid() == false);
}