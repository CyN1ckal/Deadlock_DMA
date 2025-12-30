#include "pch.h"

#include "Keybinds.h"

#include "DMA/Input Manager.h"

#include "GUI/Aimbot/Aimbot.h"

const char* CKeybind::GetKeyName(uint32_t vkCode)
{
	switch (vkCode)
	{
		// Mouse buttons
	case VK_LBUTTON: return "Left Mouse";
	case VK_RBUTTON: return "Right Mouse";
	case VK_MBUTTON: return "Middle Mouse";
	case VK_XBUTTON1: return "Mouse 4";
	case VK_XBUTTON2: return "Mouse 5";

		// Function keys
	case VK_F1: return "F1";
	case VK_F2: return "F2";
	case VK_F3: return "F3";
	case VK_F4: return "F4";
	case VK_F5: return "F5";
	case VK_F6: return "F6";
	case VK_F7: return "F7";
	case VK_F8: return "F8";
	case VK_F9: return "F9";
	case VK_F10: return "F10";
	case VK_F11: return "F11";
	case VK_F12: return "F12";

		// Special keys
	case VK_INSERT: return "Insert";
	case VK_DELETE: return "Delete";
	case VK_HOME: return "Home";
	case VK_END: return "End";
	case VK_PRIOR: return "Page Up";
	case VK_NEXT: return "Page Down";

		// Arrow keys
	case VK_LEFT: return "Left Arrow";
	case VK_RIGHT: return "Right Arrow";
	case VK_UP: return "Up Arrow";
	case VK_DOWN: return "Down Arrow";

		// Number keys
	case 0x30: return "0";
	case 0x31: return "1";
	case 0x32: return "2";
	case 0x33: return "3";
	case 0x34: return "4";
	case 0x35: return "5";
	case 0x36: return "6";
	case 0x37: return "7";
	case 0x38: return "8";
	case 0x39: return "9";

		// Letter keys
	case 0x41: return "A";
	case 0x42: return "B";
	case 0x43: return "C";
	case 0x44: return "D";
	case 0x45: return "E";
	case 0x46: return "F";
	case 0x47: return "G";
	case 0x48: return "H";
	case 0x49: return "I";
	case 0x4A: return "J";
	case 0x4B: return "K";
	case 0x4C: return "L";
	case 0x4D: return "M";
	case 0x4E: return "N";
	case 0x4F: return "O";
	case 0x50: return "P";
	case 0x51: return "Q";
	case 0x52: return "R";
	case 0x53: return "S";
	case 0x54: return "T";
	case 0x55: return "U";
	case 0x56: return "V";
	case 0x57: return "W";
	case 0x58: return "X";
	case 0x59: return "Y";
	case 0x5A: return "Z";

		// Numpad keys
	case VK_NUMPAD0: return "Numpad 0";
	case VK_NUMPAD1: return "Numpad 1";
	case VK_NUMPAD2: return "Numpad 2";
	case VK_NUMPAD3: return "Numpad 3";
	case VK_NUMPAD4: return "Numpad 4";
	case VK_NUMPAD5: return "Numpad 5";
	case VK_NUMPAD6: return "Numpad 6";
	case VK_NUMPAD7: return "Numpad 7";
	case VK_NUMPAD8: return "Numpad 8";
	case VK_NUMPAD9: return "Numpad 9";
	case VK_MULTIPLY: return "Numpad *";
	case VK_ADD: return "Numpad +";
	case VK_SUBTRACT: return "Numpad -";
	case VK_DECIMAL: return "Numpad .";
	case VK_DIVIDE: return "Numpad /";

		// Modifier keys
	case VK_SHIFT: return "Shift";
	case VK_LSHIFT: return "Left Shift";
	case VK_RSHIFT: return "Right Shift";
	case VK_CONTROL: return "Ctrl";
	case VK_LCONTROL: return "Left Ctrl";
	case VK_RCONTROL: return "Right Ctrl";
	case VK_MENU: return "Alt";
	case VK_LMENU: return "Left Alt";
	case VK_RMENU: return "Right Alt";

		// Other common keys
	case VK_SPACE: return "Space";
	case VK_TAB: return "Tab";
	case VK_RETURN: return "Enter";
	case VK_ESCAPE: return "Escape";
	case VK_BACK: return "Backspace";
	case VK_CAPITAL: return "Caps Lock";
	case VK_SNAPSHOT: return "Print Screen";
	case VK_SCROLL: return "Scroll Lock";
	case VK_PAUSE: return "Pause";

		// OEM keys
	case VK_OEM_1: return ";";
	case VK_OEM_PLUS: return "=";
	case VK_OEM_COMMA: return ",";
	case VK_OEM_MINUS: return "-";
	case VK_OEM_PERIOD: return ".";
	case VK_OEM_2: return "/";
	case VK_OEM_3: return "`";
	case VK_OEM_4: return "[";
	case VK_OEM_5: return "\\";
	case VK_OEM_6: return "]";
	case VK_OEM_7: return "'";

	default: return "Unknown";
	}
}

void Keybinds::Render()
{
	if (!bSettings) return;

	ImGui::Begin("Keybinds", &bSettings);

	if (c_keys::IsInitialized() == false)
		ImGui::Text("Input Manager not initialized!");

	Aimbot.Render();

	ImGui::End();
}

void Keybinds::OnDMAFrame(DMA_Connection* Conn)
{
	if (Aimbot.IsActive(Conn))
		Aimbot::OnFrame(Conn);
}

void CKeybind::Render()
{
	ImGui::Text("%s:", m_Name.c_str());
	ImGui::SameLine();

	if (m_bWaitingForKey)
	{
		if (ImGui::Button(("Press any key...##" + m_Name).c_str()))
			m_bWaitingForKey = false;

		for (int vk = 0; vk < 256; vk++)
		{
			// Skip mouse buttons 1-3 (can interfere with UI)
			if (vk == VK_LBUTTON || vk == VK_RBUTTON || vk == VK_MBUTTON)
				continue;

			if (GetAsyncKeyState(vk) & 0x8000)  // Key is pressed
			{
				m_Key = vk;
				m_bWaitingForKey = false;
				break;
			}
		}
	}
	else
	{
		std::string buttonLabel = std::string(GetKeyName(m_Key)) + "##" + m_Name;
		if (ImGui::Button(buttonLabel.c_str()))
			m_bWaitingForKey = true;
	}

	ImGui::SameLine();
	ImGui::Checkbox(("##Target" + m_Name).c_str(), &m_bTargetPC);
	ImGui::SameLine();
	ImGui::Checkbox(("##Radar" + m_Name).c_str(), &m_bRadarPC);
}

const bool CKeybind::IsActive(DMA_Connection* Conn) const
{
	if (m_bTargetPC && c_keys::IsKeyDown(Conn, m_Key) & 0x1)
		return true;

	if (m_bRadarPC && GetAsyncKeyState(m_Key) & 0x1)
		return true;

	return false;
}