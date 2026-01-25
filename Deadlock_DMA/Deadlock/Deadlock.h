#pragma once
#pragma once
#include "Engine/Matrix.h"
#include "Engine/Vector4.h"
#include "Engine/Vector3.h"
#include "Engine/Vector2.h"

class Deadlock
{
public:
	static bool Initialize(DMA_Connection* Conn);
	static Process& Proc();

private:
	static inline Process m_DeadlockProc{};

public:
	static inline std::mutex ViewMatrixMutex{};
	static inline Matrix44 m_ViewMatrix{ 0.0f };
	static void UpdateViewMatrix(DMA_Connection* Conn);
	static Matrix44 GetViewMatrix();
	static bool WorldToScreen(const Vector3& Pos, Vector2& ScreenPos);

public:
	static inline std::mutex m_LocalAddressMutex{};
	static inline uintptr_t m_LocalPlayerControllerAddress = 0;
	static inline uintptr_t m_LocalPlayerPawnAddress = 0;
	static bool UpdateLocalPlayerAddresses(DMA_Connection* Conn);

public:
	static inline std::mutex m_ServerTimeMutex{};
	static inline uintptr_t m_PredictionAddress = 0;
	static inline float m_ServerTime = 0.0f;
	static void GetPredictionAddress(DMA_Connection* Conn);
	static void UpdateServerTime(DMA_Connection* Conn);


private:
	static inline std::mutex m_ClientYawMutex{};
	static inline float m_ClientYaw = 0.0f;

public:
	static void UpdateClientYaw(DMA_Connection* Conn);
	static void SetClientYaw(float NewYaw);
	static float GetClientYaw();
	static float GetClientYawDegrees();
};