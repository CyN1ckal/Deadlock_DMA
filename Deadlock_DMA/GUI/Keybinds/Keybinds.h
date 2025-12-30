#pragma once

class CKeybind
{
public:
	std::string m_Name{};
	uint32_t m_Key{ 0 };
	bool m_bTargetPC{ false };
	bool m_bRadarPC{ false };
	bool m_bWaitingForKey{ false };

public:
	void Render();
	const bool IsActive(DMA_Connection* Conn) const;
	const char* GetKeyName(uint32_t vkCode);
};

class Keybinds
{
public:
	static void Render();
	static void OnDMAFrame(DMA_Connection* Conn);

public:
	static inline bool bSettings{ true };
	static inline CKeybind Debug = { "Debug", VK_F12, true, true };
	static inline CKeybind Aimbot = { "Aimbot", VK_XBUTTON2, true, true };
};