#include "pch.h"

#include "Radar.h"

#include "Deadlock/Deadlock.h"

#include "Deadlock/Entity List/EntityList.h"

#include "GUI/Color Picker/Color Picker.h"

void Radar::Render()
{
	if (!bDrawRadar) return;

	ZoneScoped;

	ImGui::Begin("Radar");

	DrawEntities();

	ImGui::End();
}

void Radar::RenderSettings()
{
	if (!bDrawRadarSettings) return;

	ImGui::Begin("Radar Settings");

	ImGui::Checkbox("Enable Radar", &bDrawRadar);

	ImGui::SliderFloat("Radar Scale", &fRadarScale, 1.0f, 50.0f, "%.1f");

	ImGui::SliderFloat("Ray Size", &fRaySize, 50.0f, 500.0f, "%.1f");

	ImGui::End();
}

void Radar::DrawEntities()
{
	ZoneScoped;

	auto DrawList = ImGui::GetWindowDrawList();
	auto WindowPos = ImGui::GetWindowPos();
	auto WindowSize = ImGui::GetWindowSize();

	ImVec2 Center = { WindowPos.x + (WindowSize.x / 2.0f), WindowPos.y + (WindowSize.y / 2.0f) };

	DrawLocalPlayer(DrawList, Center);

	std::scoped_lock Lock(EntityList::m_PawnMutex, EntityList::m_ControllerMutex);

	if (EntityList::m_LocalPawnIndex < 0) return;

	auto& LocalPawn = EntityList::m_PlayerPawns[EntityList::m_LocalPawnIndex];

	Vector3& LocalPlayerPos = LocalPawn.m_Position;

	for (auto& Pawn : EntityList::m_PlayerPawns)
	{
		if (Pawn.IsInvalid() || Pawn.IsDormant() || Pawn.IsLocalPlayer())
			continue;

		Vector3 RawRelativePos = { Pawn.m_Position.x - LocalPlayerPos.x, Pawn.m_Position.y - LocalPlayerPos.y, Pawn.m_Position.z - LocalPlayerPos.z };

		ImVec2 EntityDrawPos = { Center.x - (RawRelativePos.x / fRadarScale), Center.y + (RawRelativePos.y / fRadarScale) };

		ImU32 Color = Pawn.IsFriendly() ? ColorPicker::FriendlyRadarColor : ColorPicker::EnemyRadarColor;

		DrawList->AddCircleFilled(EntityDrawPos, 5.0f, Color);
	}
}

inline float DegToRad(float Deg)
{
	return Deg * 0.01745329f;
}

void Radar::DrawLocalPlayer(ImDrawList* DrawList, const ImVec2& Center)
{
	DrawList->AddCircleFilled(Center, 5.0f, IM_COL32(0, 255, 0, 255));

	float Yaw = 0.0f;
	{
		std::scoped_lock Lock(Deadlock::m_ClientYawMutex);
		Yaw = Deadlock::m_ClientYaw;
	}
	Yaw += 180.0f;

	float Rad = DegToRad(Yaw);

	ImVec2 LineEnd = { Center.x + (fRaySize * std::cos(Rad)), Center.y - (fRaySize * std::sin(Rad)) };
	DrawList->AddLine(Center, LineEnd, IM_COL32(0, 255, 0, 255), 2.0f);
}
