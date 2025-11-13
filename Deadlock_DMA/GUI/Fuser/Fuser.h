#pragma once

class Fuser
{
public:
	static void OnFrame();
	static void RenderSettings();
	static void RenderSoulsPerMinute();

public:
	static inline ImVec2 ScreenSize = { 1920.0f,1080.0f };
	static inline bool bDrawSoulsPerMinute = true;
	static inline bool bMasterToggle = true;
};