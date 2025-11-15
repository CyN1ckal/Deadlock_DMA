#pragma once

#include "makcu/makcu.h"
#include "Deadlock/Const/Aimpoints.h"

class Aimbot
{
public:
	static void RenderSettings();
	static void OnFrame(DMA_Connection* Conn);
	static void RenderFOVCircle();

private:
	static inline makcu::Device m_Device{};
	static inline float fDampen{ 1.0f };
	static inline float fMaxPixelDistance{ 100.0f };
	static inline float fBulletVelocity{ 500.0f };
	static inline bool bAimHead{ true };
	static inline bool bDrawMaxFOV{ true };
	static inline bool bPrediction{ true };

private:
	static Vector2 GetAimDelta(const Vector2& CenterScreen);
};