#include "pch.h"

#include "Keybinds.h"

#include "DMA/Input Manager.h"

#include "GUI/Aimbot/Aimbot.h"

void Keybinds::Render()
{
	ImGui::Begin("Keybinds");

	if (c_keys::IsInitialized())
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Keyboard Connected!");

	ImGui::PushItemWidth(50.0f);

	ImGui::InputScalar("Debug Key", ImGuiDataType_U32, &m_DebugHotkey);

	ImGui::InputScalar("Aimbot Key", ImGuiDataType_U32, &m_AimbotHotkey);

	ImGui::PopItemWidth();

	ImGui::End();
}

void Keybinds::OnFrame(DMA_Connection* Conn)
{
	if (c_keys::IsKeyDown(Conn, m_DebugHotkey))
		std::println("Debug Key Pressed!");

	if (c_keys::IsKeyDown(Conn, m_AimbotHotkey))
		Aimbot::OnFrame(Conn);
}
