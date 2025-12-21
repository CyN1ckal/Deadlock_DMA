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
	static inline float DegToRad(float Deg)
	{
		return Deg * 0.01745329f;
	};
	static inline ImVec2 ClampToRect(ImVec2 p, ImVec2 tl, ImVec2 br)
	{
		if (p.x < tl.x) p.x = tl.x;
		if (p.y < tl.y) p.y = tl.y;
		if (p.x > br.x) p.x = br.x;
		if (p.y > br.y) p.y = br.y;
		return p;
	};
public:
	static inline bool bDrawRadar{ true };
	static inline bool bDrawRadarSettings{ true };
	static inline bool bHideFriendly{ false };
	static inline bool bMobaStyle{ false };
	static inline float fRadarScale{ 10.0f };
	static inline float fRaySize{ 100.0f };
	static inline float fRadarPadding = 10.0f;
};