#include "pch.h"

#include "Deadlock.h"

#include "Entity List/EntityList.h"

bool Deadlock::Initialize(DMA_Connection* Conn)
{
	auto& Process = Deadlock::Proc();

	Process.GetProcessInfo("deadlock.exe", Conn);

	FirstEntityList::UpdateCrucialInformation(Conn, &Process);

	FirstEntityList::UpdateEntityMap(Conn, &Process);

	FirstEntityList::UpdatePlayerControllerAddresses();

	FirstEntityList::UpdatePlayerControllers(Conn, &Process);

	FirstEntityList::UpdatePlayerPawnAddresses();

	FirstEntityList::UpdatePlayerPawns(Conn, &Process);

	UpdateLocalPlayerControllerAddress(Conn);

	return false;
}

Process& Deadlock::Proc()
{
	return m_DeadlockProc;
}

void Deadlock::UpdateViewMatrix(DMA_Connection* Conn)
{
	std::scoped_lock Lock(ViewMatrixMutex);
	uintptr_t ViewMatrixAddress = Proc().GetClientBase() + Offsets::ViewMatrix;
	m_ViewMatrix = Proc().ReadMem<Matrix44>(Conn, ViewMatrixAddress);
}

const Vector2 ScreenSize{ 1920.0f, 1080.0f };
bool Deadlock::WorldToScreen(const Vector3& Pos, Vector2& ScreenPos)
{
	std::scoped_lock Lock(ViewMatrixMutex);

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

bool Deadlock::UpdateLocalPlayerControllerAddress(DMA_Connection* Conn)
{
	uintptr_t LocalPlayerControllerAddress = Proc().GetClientBase() + Offsets::LocalController;
	m_LocalPlayerControllerAddress = Proc().ReadMem<uintptr_t>(Conn, LocalPlayerControllerAddress);

	return false;
}
