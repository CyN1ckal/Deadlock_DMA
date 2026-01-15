#pragma once

class ColorPicker
{
public:
	static void Render();
	static void MyColorPicker(const char* label, ImColor& color);

public:
	static inline bool bMasterToggle{ true };
	static inline ImColor FriendlyNameTagColor{ 0.29f,0.619f,0.6f,1.0f };
	static inline ImColor EnemyNameTagColor{ 0.776f,0.223f,0.247f,1.0f };
	static inline ImColor EnemyBoneColor{ 1.0f,0.0f,0.0f,1.0f };
	static inline ImColor FriendlyBoneColor{ 0.0f,1.0f,1.0f,1.0f };
	static inline ImColor SinnersColor{ 0.65f,0.05f,0.7f,1.0f };
	static inline ImColor MonsterCampColor{ 0.8f,0.8f,0.8f,1.0f };
	static inline ImColor FriendlyRadarColor{ 0.0f,1.0f,1.0f,1.0f };
	static inline ImColor EnemyRadarColor{ 1.0f,0.0f,0.0f,1.0f };
	static inline ImColor FriendlyTrooperColor{ 0.31f,0.79f,1.0f,1.0f };
	static inline ImColor EnemyTrooperColor{ 1.0f,0.33f,0.33f,1.0f };
	static inline ImColor UnsecuredSoulsTextColor{ 1.0f,1.0f,1.0f,1.0f };
	static inline ImColor UnsecuredSoulsHighlightedTextColor{ 1.0f,1.0f,0.0f,1.0f };
	static inline ImColor FriendlyHealthStatusBarColor{ 0.0f,0.8f,0.0f,1.0f };
	static inline ImColor EnemyHealthStatusBarColor{ 0.8f,0.0f,0.0f,1.0f };
	static inline ImColor FriendlySoulsStatusBarColor{ 0.0f,0.5f,1.0f,1.0f };
	static inline ImColor EnemySoulsStatusBarColor{ 1.0f,0.5f,0.0f,1.0f };
	static inline ImColor HealthBarForegroundColor{ 0.0f,0.8f,0.0f,1.0f };
	static inline ImColor HealthBarBackgroundColor{ 0.2f,0.2f,0.2f,1.0f };
	static inline ImColor AimbotFOVCircle{ 1.0f,0.0f,0.0f,1.0f };
	static inline ImColor RadarBackgroundColor{ 0.0f,0.0f,0.0f,1.0f };
};