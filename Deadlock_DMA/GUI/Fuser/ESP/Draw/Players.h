#pragma once
#include "Deadlock/Classes/Classes.h"

class Draw_Players
{
public:
	static void operator()();

public:
	static inline bool bMasterToggle{ true };
	static inline bool bHideFriendly{ false };
	static inline bool bDrawBones{ true };
	static inline float fBonesThickness{ 3.f };
	static inline bool bDrawHead{ true };
	static inline bool bDrawVelocityVector{ false };
	static inline bool bDrawUnsecuredSouls{ true };
	static inline int32_t UnsecuredSoulsMinimumThreshold{ 1 };
	static inline int32_t UnsecuredSoulsHighlightThreshold{ 400 };
	static inline bool bBoneNumbers{ false };
	static inline bool bDrawHealthBar{ true };
	static inline bool bHideLocalPlayer{ true };
	static inline bool bShowDistance{ true };

private:
	static void DrawPlayer(const CCitadelPlayerController& PC, const CCitadelPlayerPawn& Pawn);
	static void DrawHealthBar(const CCitadelPlayerController& PC, const CCitadelPlayerPawn& Pawn, const ImVec2& PawnScreenPos, ImDrawList* DrawList, int& LineNumber);
	static void DrawSkeleton(const CCitadelPlayerController& PC, const CCitadelPlayerPawn& Pawn, ImDrawList* DrawList, const ImVec2& WindowPos);
	static void DrawHeadCircle(const CCitadelPlayerController& PC, const CCitadelPlayerPawn& Pawn, ImDrawList* DrawList, const ImVec2& WindowPos);
	static void DrawVelocityVector(const CCitadelPlayerPawn& Pawn, ImDrawList* DrawList, const ImVec2& WindowPos);
	static void DrawBoneNumbers(const CCitadelPlayerPawn& Pawn);
	static void DrawNameTag(const CCitadelPlayerController& PC, const CCitadelPlayerPawn& Pawn, ImDrawList* DrawList, const ImVec2& WindowPos, int& LineNumber);
	static void DrawUnsecuredSouls(const CCitadelPlayerPawn& Pawn, const Vector2& ScreenPos, int& LineNumber);
};