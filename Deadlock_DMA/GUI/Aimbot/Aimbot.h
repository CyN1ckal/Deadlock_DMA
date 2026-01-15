#pragma once
#include "Deadlock/Const/Aimpoints.h"

class Aimbot
{
public:
	static void RenderSettings();
	static void OnFrame(DMA_Connection* Conn);
	static void RenderFOVCircle();

public:
	static inline bool bSettings{ false };
	static inline bool bMasterToggle{ false };

	static inline float fSmoothX{ 10.0f };           // Higher = smoother (1-50)
	static inline float fSmoothY{ 10.0f };           // Higher = smoother (1-50)
	static inline bool bIndependentSmoothing{ false };

	static inline bool bHumanization{ true };
	static inline float fRandomness{ 1.0f };         // 0-5
	static inline float fMicroCorrection{ 0.5f };    // 0-2
	static inline float fOvershootChance{ 15.0f };   // 0-100%
	static inline float fOvershootAmount{ 5.0f };    // 1-15%
	static inline bool bDistanceBasedSmoothing{ true };

	static inline float fMaxPixelDistance{ 100.0f };
	static inline bool bAimHead{ true };
	static inline bool bDrawMaxFOV{ true };

	static inline bool bPrediction{ true };
	static inline float fBulletVelocity{ 500.0f };

private:
	static Vector2 GetAimDelta(const Vector2& CenterScreen);
	static Vector2 ApplyHumanization(Vector2 MoveAmount, float Distance, bool& bOvershootActive, Vector2& CurrentOffset);
};