#pragma once

#include "makcu/makcu.h"
#include "Deadlock/Const/Aimpoints.h"

class Aimbot
{
public:
	static void Render();
	static void OnFrame(DMA_Connection* Conn);

private:
	static inline makcu::Device m_Device;
	static inline float fDampen = 1.0f;
	static inline float fMaxPixelDistance = 100.0f;

private:
	static Vector2 GetAimDelta(const Vector2& CenterScreen);
};