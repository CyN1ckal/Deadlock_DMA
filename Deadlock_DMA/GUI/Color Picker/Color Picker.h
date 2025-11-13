#pragma once

class ColorPicker
{
public:
	static void RenderColorPicker();

public:
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
};