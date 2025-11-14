#pragma once

class Keybinds
{
public:
	static void Render();
	static void OnFrame(DMA_Connection* Conn);

public:

private:
	static inline uint32_t m_DebugHotkey = VK_F12;
};