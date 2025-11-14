#pragma once

class Keybinds
{
public:
	static void Render();
	static void OnFrame(DMA_Connection* Conn);

public:
	static inline uint32_t m_DebugHotkey = VK_F12;
	static inline uint32_t m_AimbotHotkey = VK_XBUTTON2;
};