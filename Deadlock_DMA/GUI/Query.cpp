#include "pch.h"
#include "Query.h"
#include "GUI/Fuser/Fuser.h"
#include "GUI/Fuser/ESP/Draw/Players.h"
#include "GUI/Fuser/ESP/Draw/Troopers.h"
#include "GUI/Fuser/ESP/Draw/Camps.h"
#include "GUI/Fuser/ESP/Draw/Sinners.h"

bool Query::IsUsingPlayers()
{
	if (Fuser::bMasterToggle && Draw_Players::bMasterToggle) return true;

	return false;
}

bool Query::IsUsingTroopers()
{
	if (Fuser::bMasterToggle && Draw_Troopers::bMasterToggle) return true;

	return false;
}

bool Query::IsUsingCamps()
{
	if (Fuser::bMasterToggle && Draw_Camps::bMasterToggle) return true;

	return false;
}

bool Query::IsUsingSinners()
{
	if (Fuser::bMasterToggle && Draw_Sinners::bMasterToggle) return true;

	return false;
}