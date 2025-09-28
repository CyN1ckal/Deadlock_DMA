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
	static inline Matrix44 m_ViewMatrix { 0.0f };
	static void UpdateViewMatrix(DMA_Connection* Conn);
	static bool WorldToScreen(const Vector3& Pos, Vector2& ScreenPos);
};