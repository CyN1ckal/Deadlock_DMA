#pragma once
#include "Deadlock/Classes/Classes.h"

class Radar
{
public:
	static void Render();
	static void RenderSettings();
private:
	static void DrawEntities();
	static void DrawLocalPlayerViewRay(ImDrawList* DrawList, const ImVec2& ScreenPos, const ETeam& LocalTeam);
    static void DrawPlayer(const CCitadelPlayerPawn& Pawn, const ImVec2& RadarPos);
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
	static void DrawRadarBackground();

public:
	static inline bool bMasterToggle{ true };
	static inline bool bHideFriendly{ false };
	static inline bool bMobaStyle{ false };
	static inline float fRadarScale{ 10.0f };
	static inline float fRaySize{ 100.0f };
	static inline bool bPlayerCentered{ false };

private:
	static Vector3 GetRadarCenterScreenPos();
	static ImVec2 GetUVFromCoords(const Vector3& GameCoords, ETeam LocalTeam);
	static inline ImVec2 MapSize{ 20600.0f, 20600.0f };
	static ImVec2 GetLocalPlayerScreenPos(const ImVec2& RadarWindowCenter, const Vector3& RadarCenterGamePos);
	static 	Vector3 FindRadarTopLeftCoords(const Vector3& CenterRadarGamePosition, const ETeam& LocalTeam);
	static 	Vector3 FindRadarBottomRightCoords(const Vector3& CenterRadarGamePosition, const ETeam& LocalTeam);
	static ImVec2 GetRadarSizeInGameUnits();
};