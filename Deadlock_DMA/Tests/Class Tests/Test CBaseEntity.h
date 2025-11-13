#pragma once
#include "Deadlock/Classes/CBaseEntity.h"

TEST_CASE("CBaseEntity::Missing m_EntityAddress")
{
	CBaseEntity Entity(INVALID_ADDRESS);
	Entity.PrepareRead_1(TEST_VMSH);
	REQUIRE(Entity.IsInvalid() == true);
}

TEST_CASE("CBaseEntity::Valid m_EntityAddress")
{
	CBaseEntity Entity(VALID_ADDRESS);
	Entity.PrepareRead_1(TEST_VMSH);
	REQUIRE(Entity.IsInvalid() == false);
}

TEST_CASE("CBaseEntity:: Missing m_GameSceneNodeAddress")
{
	CBaseEntity Entity(VALID_ADDRESS);
	Entity.PrepareRead_2(TEST_VMSH);
	REQUIRE(Entity.IsInvalid() == true);
}

TEST_CASE("CBaseEntity:: Valid m_GameSceneNodeAddress")
{
	CBaseEntity Entity(VALID_ADDRESS);
	Entity.m_GameSceneNodeAddress = VALID_ADDRESS;
	Entity.PrepareRead_2(TEST_VMSH);
	REQUIRE(Entity.IsInvalid() == false);
}