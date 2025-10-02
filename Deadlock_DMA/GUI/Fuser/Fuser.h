#pragma once

class Fuser
{
public:
	static void OnFrame();
	static void RenderSettings();

public:
	static inline ImVec2 ScreenSize = { 1920.0f,1080.0f };
	static inline bool bFullScreen = false;
};