#pragma once

class Radar
{
public:
	static void Render();
	static void RenderSettings();

private:
	static void DrawEntities();
	static void DrawLocalPlayer(ImDrawList* DrawList, const ImVec2& Center);

public:
	static inline bool bDrawRadar{ true };
	static inline bool bDrawRadarSettings{ true };
	static inline float fRadarScale{ 10.0f };
	static inline float fRaySize{ 100.0f };
};