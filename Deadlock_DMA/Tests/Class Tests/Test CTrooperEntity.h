#pragma once

TEST_CASE("CTrooperEntity::Missing m_EntityAddress")
{
	CTrooperEntity Entity(INVALID_ADDRESS);
	Entity.PrepareRead_1(TEST_VMSH);
	REQUIRE(Entity.IsInvalid() == true);
}

TEST_CASE("CTrooperEntity::Valid m_EntityAddress")
{
	CTrooperEntity Entity(VALID_ADDRESS);
	Entity.PrepareRead_1(TEST_VMSH);
	REQUIRE(Entity.IsInvalid() == false);
}

TEST_CASE("CTrooperEntity::<1 Health")
{
	CTrooperEntity Entity(VALID_ADDRESS);
	Entity.m_GameSceneNodeAddress = VALID_ADDRESS;
	Entity.m_CurrentHealth = 0;
	Entity.PrepareRead_2(TEST_VMSH);
	REQUIRE(Entity.IsInvalid() == true);
}

TEST_CASE("CTrooperEntity::>= 1 Health")
{
	CTrooperEntity Entity(VALID_ADDRESS);
	Entity.m_GameSceneNodeAddress = VALID_ADDRESS;
	Entity.m_CurrentHealth = 1;
	Entity.PrepareRead_2(TEST_VMSH);
	REQUIRE(Entity.IsInvalid() == false);
}