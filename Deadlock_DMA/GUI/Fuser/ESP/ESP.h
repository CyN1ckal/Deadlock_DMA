#pragma once

#include "Deadlock/Classes/Classes.h"

class CNameTagSetings
{
public:
	bool bDrawNameTags{ true };
	bool bShowLevel{ true };
	bool bShowHeroName{ true };
	bool bShowDistance{ true };
	bool bShowHealth{ true };
	bool bHideFriendly{ true };
};

class CSkeletonSettings
{
public:
	bool bDrawSkeleton{ true };
	bool bHideFriendly{ true };
};

class ESP
{
public:
	static void OnFrame();
	static void RenderSettings();

public:
	static inline bool bBoneNumbers{ false };
	static inline bool bHideLocal{ true };
	static inline bool bDrawTroopers{ true };
	static inline bool bDrawCamps{ true };
	static inline bool bDrawSinners{ true };
	static inline CNameTagSetings NameTagSettings{};
	static inline CSkeletonSettings SkeletonSettings{};

private:
	static void RenderPlayers(const ImVec2 WindowPos, ImDrawList* DrawList);
	static void RenderTroopers();
	static void RenderMonsterCamps();
	static void RenderSinners();

	static void DrawTrooper(CBaseEntity& Trooper);

};