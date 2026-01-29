#pragma once

#include "makcu/makcu.h"
#include "Deadlock/Const/Aimpoints.h"

class Aimbot
{
public:
	static void RenderSettings();
	static void OnFrame(DMA_Connection* Conn);
	static void RenderFOVCircle();

public:
	static inline bool bSettings{ true };
	static inline float fSmoothX{ 15.0f };
	static inline float fSmoothY{ 18.0f };
	static inline float fGaussianNoise{ 0.8f }; 
	static inline float fMaxPixelDistance{ 100.0f };
	static inline float fBulletVelocity{ 500.0f };
	static inline bool bAimHead{ true };
	static inline bool bDrawMaxFOV{ true };
	static inline bool bPrediction{ true };

private:
	static Vector2 GetAimDelta(const Vector2& CenterScreen);
	static inline std::random_device rd;
	static inline std::mt19937 gen{ rd() };
	static inline bool bIsActive = false;
};