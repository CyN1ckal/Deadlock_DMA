#pragma once

class Fuser
{
public:
	static void Render();
	static void RenderSettings();
	static void RenderSoulsPerMinute();

public:
	static inline bool bSettings{ true };
	static inline bool bMasterToggle{ true };
	static inline bool bDrawSoulsPerMinute{ true };
	static inline ImVec2 m_ScreenSize{ 1920.0f,1080.0f };
};