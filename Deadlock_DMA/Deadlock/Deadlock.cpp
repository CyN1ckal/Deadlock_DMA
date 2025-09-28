#include "pch.h"

#include "Deadlock.h"

#include "Entity List/EntityList.h"

bool Deadlock::Initialize(DMA_Connection* Conn)
{
	auto& Process = Deadlock::Proc();

	Process.GetProcessInfo("deadlock.exe", Conn);

	EntityList::UpdateCrucialInformation(Conn, &Process);

	EntityList::UpdateEntityMap(Conn, &Process);

	EntityList::UpdatePlayerControllerAddresses();

	EntityList::UpdatePlayerControllers(Conn, &Process);

	EntityList::UpdatePlayerPawnAddresses();

	EntityList::UpdatePlayerPawns(Conn, &Process);

	EntityList::PrintPlayerPawns();

	Deadlock::UpdateViewMatrix(Conn);

	Vector2 ScreenPosition;
	WorldToScreen(Vector3{ 0.0f,0.0f,0.0f }, ScreenPosition);
	std::println("Screen Position: X: {}, Y: {}", ScreenPosition.x, ScreenPosition.y);

	return false;
}

Process& Deadlock::Proc()
{
	return m_DeadlockProc;
}

void Deadlock::UpdateViewMatrix(DMA_Connection* Conn)
{
	uintptr_t ViewMatrixAddress = Proc().GetClientBase() + Offsets::ViewMatrix;
	m_ViewMatrix = Proc().ReadMem<Matrix44>(Conn, ViewMatrixAddress);
}

const Vector2 ScreenSize{ 1920.0f, 1080.0f };
bool Deadlock::WorldToScreen(const Vector3& Pos, Vector2& ScreenPos)
{
	ScreenPos.x = m_ViewMatrix.m00 * Pos.x + m_ViewMatrix.m01 * Pos.y + m_ViewMatrix.m02 * Pos.z + m_ViewMatrix.m03;
	ScreenPos.y = m_ViewMatrix.m10 * Pos.x + m_ViewMatrix.m11 * Pos.y + m_ViewMatrix.m12 * Pos.z + m_ViewMatrix.m13;

	float w = m_ViewMatrix.m30 * Pos.x + m_ViewMatrix.m31 * Pos.y + m_ViewMatrix.m32 * Pos.z + m_ViewMatrix.m33;

	if (w < 0.01f)
		return false;

	float inv_w = 1.f / w;
	ScreenPos.x *= inv_w;
	ScreenPos.y *= inv_w;

	float x = ScreenSize.x * .5f;
	float y = ScreenSize.y * .5f;

	x += 0.5f * ScreenPos.x * ScreenSize.x + 0.5f;
	y -= 0.5f * ScreenPos.y * ScreenSize.y + 0.5f;

	ScreenPos.x = x;
	ScreenPos.y = y;

	return true;
}
