#pragma once

class ColorPicker
{
public:
	static void RenderColorPicker();

public:
	// friend
	static inline ImVec4 FriendlyNameTagColor{ 0.05f, 0.45f, 0.25f, 1.0f };  // dark teal/forest
	static inline ImVec4 FriendlyBoneColor{ 0.20f, 0.70f, 0.90f, 1.0f };  // sky blue

	// enemy
	static inline ImVec4 EnemyNameTagColor{ 0.60f, 0.10f, 0.15f, 1.0f };  // deep crimson, white text visible
	static inline ImVec4 EnemyBoneColor{ 0.95f, 0.20f, 0.20f, 1.0f };  // vivid red
};