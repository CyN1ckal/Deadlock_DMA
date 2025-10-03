#pragma once

#include "Deadlock/Classes/Classes.h"

class ESP
{
public:
	static void OnFrame();
	static void RenderSettings();

public:
	static inline bool bHideFriendly = true;
	static inline bool bBoneNumbers = false;
	static inline bool bDrawBones = true;
	static inline bool bHideLocal = true;
	static inline bool bDrawNameTags = true;
	static inline bool bDrawDistance = true;

private:
	static void DrawBoneNumers(CCitadelPlayerPawn& Pawn);
};