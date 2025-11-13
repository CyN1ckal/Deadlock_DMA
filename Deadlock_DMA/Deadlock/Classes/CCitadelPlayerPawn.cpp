#include "pch.h"
#include "CCitadelPlayerPawn.h"

//void CCitadelPlayerPawn::DrawBoneNumbers() const
//{
//	ImGui::PushFont(nullptr, 12.0f);
//
//	for (int i = 0; i < MAX_BONES; i++)
//	{
//		Vector2 ScreenPos{};
//		if (!Deadlock::WorldToScreen(m_BonePositions[i], ScreenPos)) continue;
//
//		std::string BoneString = std::to_string(i);
//		auto TextSize = ImGui::CalcTextSize(BoneString.c_str());
//
//		ImGui::SetCursorPos(ImVec2(ScreenPos.x - (TextSize.x / 2.0f), ScreenPos.y));
//		ImGui::Text(BoneString.c_str());
//	}
//
//	ImGui::PopFont();
//}