#pragma once

class ColorPicker
{
public:
	static void RenderColorPicker();

public:
	static inline ImVec4 FriendlyNameTagColor{ 0.29f,0.619f,0.6f,1.0f };
	static inline ImVec4 EnemyNameTagColor{ 0.776f,0.223f,0.247f,1.0f };
	static inline ImVec4 EnemyBoneColor{ 1.0f,0.0f,0.0f,1.0f };
	static inline ImVec4 FriendlyBoneColor{ 0.0f,1.0f,1.0f,1.0f };
	static inline ImVec4 SinnersColor{ 0.65f,0.05f,0.7f,1.0f };
	static inline ImVec4 MonsterCampColor{ 0.8f,0.8f,0.8f,1.0f };
	static inline ImVec4 FriendlyRadarColor{ 0.0f,1.0f,1.0f,1.0f };
	static inline ImVec4 EnemyRadarColor{ 1.0f,0.0f,0.0f,1.0f };
};