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
};

class ESP
{
public:
	static void OnFrame();
	static void RenderSettings();

public:
	static inline bool bHideFriendly{ true };
	static inline bool bBoneNumbers{ false };
	static inline bool bDrawBones{ true };
	static inline bool bHideLocal{ true };
	static inline bool bDrawTroopers{ true };
	static inline bool bDrawCamps{ true };
	static inline CNameTagSetings NameTagSettings{};

private:
	static void RenderPlayers(const ImVec2 WindowPos, ImDrawList* DrawList);
	static void RenderTroopers();
	static void RenderMonsterCamps();

	static void DrawTrooper(CBaseEntity& Trooper);

};