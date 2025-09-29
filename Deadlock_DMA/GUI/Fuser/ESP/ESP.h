#pragma once

class ESP
{
public:
	static void OnFrame();
	static void RenderSettings();

public:
	static inline ImVec4 FriendlyColor{ 0.29f,0.619f,0.6f,1.0f };
	static inline ImVec4 EnemyColor{ 0.776f,0.223f,0.247f,1.0f };
	static inline bool bHideFriendly = true;
};