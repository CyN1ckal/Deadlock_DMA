#pragma once
#include "Deadlock/Classes/Classes.h"

class Radar
{
public:
	static void Render();
	static void RenderSettings();
private:
	static void DrawEntities();
	static void DrawLocalPlayer(ImDrawList* DrawList, const ImVec2& Center);
    static void DrawPlayer(const CCitadelPlayerController& PC, const CCitadelPlayerPawn& Pawn, const ImVec2& RadarPos);
    static void DrawNameTag(const CCitadelPlayerController& PC, const CCitadelPlayerPawn& Pawn, ImDrawList* DrawList, const ImVec2& AnchorPos, int& LineNumber);
	static void DrawHealthBar(const CCitadelPlayerController& PC, const CCitadelPlayerPawn& Pawn, ImDrawList* DrawList, const ImVec2& AnchorPos, int& LineNumber);
public:
	static inline bool bDrawRadar{ true };
	static inline bool bDrawRadarSettings{ true };
	static inline bool bHideFriendly{ true };
	static inline float fRadarScale{ 10.0f };
	static inline float fRaySize{ 100.0f };
};