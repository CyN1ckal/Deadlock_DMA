#pragma once

class ColorPicker
{
public:
	static void Render();
	static void MyColorPicker(const char* label, ImColor& color);

public:
	static inline bool bMasterToggle{ true };
	static inline ImColor SinnersColor{ 0.65f,0.05f,0.7f,1.0f };
	static inline ImColor MonsterCampColor{ 0.8f,0.8f,0.8f,1.0f };
	static inline ImColor LocalPlayerRadar{ 0.0f,1.0f,0.0f,1.0f };
	static inline ImColor UnsecuredSoulsTextColor{ 1.0f,1.0f,1.0f,1.0f };
	static inline ImColor UnsecuredSoulsHighlightedTextColor{ 1.0f,1.0f,0.0f,1.0f };
	static inline ImColor FriendlyHealthStatusBarColor{ 0.0f,0.8f,0.0f,1.0f };
	static inline ImColor EnemyHealthStatusBarColor{ 0.8f,0.0f,0.0f,1.0f };
	static inline ImColor FriendlySoulsStatusBarColor{ 0.0f,0.5f,1.0f,1.0f };
	static inline ImColor EnemySoulsStatusBarColor{ 1.0f,0.5f,0.0f,1.0f };
	static inline ImColor HealthBarForegroundColor{ 0.0f,0.8f,0.0f,1.0f };
	static inline ImColor HealthBarBackgroundColor{ 0.2f,0.2f,0.2f,1.0f };
	static inline ImColor AimbotFOVCircle{ 1.0f,0.0f,0.0f,1.0f };
	static inline ImColor AimbotFOVCircleActive{ 0.0f, 1.0f, 0.0f, 1.0f };
	static inline ImColor RadarBackgroundColor{ 0.0f,0.0f,0.0f,1.0f };

public:
	static inline ImColor ArchMotherTeamColor{ 78, 118, 196 };
	static inline ImColor HiddenKingTeamColor{ 212, 135, 12 };
	static inline ImColor SkeletonColor{ 255,255,255 };
};