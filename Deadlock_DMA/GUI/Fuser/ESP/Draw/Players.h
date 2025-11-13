#pragma once
#include "Deadlock/Classes/Classes.h"

class Draw_Players
{
public:
	static void operator()();
	static inline bool bHideLocalPlayer{ true };

private:
	static void DrawPlayer(const CCitadelPlayerController& PC, const CCitadelPlayerPawn& Pawn);
	static void DrawHealthBar(const CCitadelPlayerController& PC, const CCitadelPlayerPawn& Pawn, const ImVec2& PawnScreenPos, ImDrawList* DrawList, int& LineNumber);
	static void DrawSkeleton(const CCitadelPlayerController& PC,const CCitadelPlayerPawn& Pawn, ImDrawList* DrawList, const ImVec2& WindowPos);
	static void DrawNameTag(const CCitadelPlayerController& PC, const CCitadelPlayerPawn& Pawn, ImDrawList* DrawList, const ImVec2& WindowPos, int& LineNumber);
};