#include "pch.h"

#include "Deadlock.h"

#include "Entity List/EntityList.h"

#include "GUI/Fuser/Fuser.h"

bool Deadlock::Initialize(DMA_Connection* Conn)
{
	auto& Process = Deadlock::Proc();

	Process.GetProcessInfo("deadlock.exe", Conn);

	EntityList::FullUpdate(Conn, &Process);

	UpdateLocalPlayerControllerAddress(Conn);

	GetPredictionAddress(Conn);

	return false;
}

Process& Deadlock::Proc()
{
	return m_DeadlockProc;
}

void Deadlock::UpdateViewMatrix(DMA_Connection* Conn)
{
	std::scoped_lock lock(ViewMatrixMutex);
	uintptr_t ViewMatrixAddress = Proc().GetClientBase() + Offsets::ViewMatrix;
	m_ViewMatrix = Proc().ReadMem<Matrix44>(Conn, ViewMatrixAddress);
}

bool Deadlock::WorldToScreen(const Vector3& Pos, Vector2& ScreenPos)
{
	std::scoped_lock lock(ViewMatrixMutex);

	ScreenPos.x = m_ViewMatrix.m00 * Pos.x + m_ViewMatrix.m01 * Pos.y + m_ViewMatrix.m02 * Pos.z + m_ViewMatrix.m03;
	ScreenPos.y = m_ViewMatrix.m10 * Pos.x + m_ViewMatrix.m11 * Pos.y + m_ViewMatrix.m12 * Pos.z + m_ViewMatrix.m13;

	float w = m_ViewMatrix.m30 * Pos.x + m_ViewMatrix.m31 * Pos.y + m_ViewMatrix.m32 * Pos.z + m_ViewMatrix.m33;

	if (w < 0.01f)
		return false;

	float inv_w = 1.f / w;
	ScreenPos.x *= inv_w;
	ScreenPos.y *= inv_w;

	float x = Fuser::ScreenSize.x * .5f;
	float y = Fuser::ScreenSize.y * .5f;

	x += 0.5f * ScreenPos.x * Fuser::ScreenSize.x + 0.5f;
	y -= 0.5f * ScreenPos.y * Fuser::ScreenSize.y + 0.5f;

	ScreenPos.x = x;
	ScreenPos.y = y;

	return true;
}

bool Deadlock::UpdateLocalPlayerControllerAddress(DMA_Connection* Conn)
{
	std::scoped_lock Lock(m_LocalAddressMutex);

	uintptr_t LocalPlayerControllerAddress = Proc().GetClientBase() + Offsets::LocalController;
	m_LocalPlayerControllerAddress = Proc().ReadMem<uintptr_t>(Conn, LocalPlayerControllerAddress);
	m_LocalPlayerPawnAddress = EntityList::GetEntityAddressFromHandle(EntityList::m_PlayerControllers[m_LocalPlayerControllerAddress].m_hPawn);

	return false;
}

void Deadlock::GetPredictionAddress(DMA_Connection* Conn)
{
	uintptr_t PredictionPtrAddress = Proc().GetClientBase() + Offsets::PredictionPtr;
	m_PredictionAddress = Proc().ReadMem<uintptr_t>(Conn, PredictionPtrAddress);
}

void Deadlock::UpdateServerTime(DMA_Connection* Conn)
{
	if (!m_PredictionAddress) return;

	uintptr_t ServerTimeAddress = m_PredictionAddress + Offsets::Prediction::ServerTime;

	std::scoped_lock lock(m_ServerTimeMutex);
	m_ServerTime = Proc().ReadMem<float>(Conn, ServerTimeAddress);
}

void Deadlock::UpdateClientYaw(DMA_Connection* Conn)
{
	std::scoped_lock Lock(m_ClientYawMutex);
	uintptr_t YawAddress = Proc().GetClientBase() + Offsets::Rotation + sizeof(float);
	m_ClientYaw = Proc().ReadMem<float>(Conn, YawAddress);
}