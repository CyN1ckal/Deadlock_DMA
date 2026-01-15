#include "pch.h"
#include "DMA/DMA.h"
#include "DMA/Input Manager.h"
#include "GUI/Keybinds/Keybinds.h"
#include "Aimbot.h"
#include "GUI/Fuser/Fuser.h"
#include "GUI/Color Picker/Color Picker.h"
#include "Deadlock/Entity List/EntityList.h"
#include "Makcu/MyMakcu.h"

void Aimbot::RenderSettings()
{
	if (!bSettings) return;

	ImGui::Begin("Aimbot Settings", &bSettings);

	ImGui::Checkbox("Enable Aimbot", &bMasterToggle);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::SeparatorText("Smoothing");
	ImGui::SliderFloat("Smooth X", &fSmoothX, 1.0f, 50.0f, "%.1f");
	ImGui::SliderFloat("Smooth Y", &fSmoothY, 1.0f, 50.0f, "%.1f");
	ImGui::Checkbox("Independent Axes", &bIndependentSmoothing);
	if (bIndependentSmoothing)
	{
		ImGui::TextDisabled("(Separate X/Y smoothing values)");
	}


	ImGui::Spacing();

	ImGui::SeparatorText("Humanization");
	ImGui::Checkbox("Enable Humanization", &bHumanization);
	if (bHumanization)
	{
		ImGui::Spacing();

		ImGui::SliderFloat("Randomness", &fRandomness, 0.0f, 5.0f, "%.2f");
		ImGui::TextDisabled("Random micro-movements");

		ImGui::SliderFloat("Micro Correction", &fMicroCorrection, 0.0f, 2.0f, "%.2f");
		ImGui::TextDisabled("Small adjustments when near target");

		ImGui::Spacing();

		ImGui::SliderFloat("Overshoot Chance", &fOvershootChance, 0.0f, 100.0f, "%.1f%%");
		ImGui::SliderFloat("Overshoot Amount", &fOvershootAmount, 1.0f, 15.0f, "%.1f%%");
		ImGui::TextDisabled("Occasionally move past target");

		ImGui::Spacing();

		ImGui::Checkbox("Distance-Based Smoothing", &bDistanceBasedSmoothing);
		ImGui::TextDisabled("Adjust speed based on target distance");
	}

	ImGui::Spacing();

	ImGui::SeparatorText("Targeting");
	ImGui::SliderFloat("FOV (Max Distance)", &fMaxPixelDistance, 10.0f, 500.0f, "%.0f px");
	ImGui::Checkbox("Draw FOV Circle", &bDrawMaxFOV);
	ImGui::Checkbox("Aim at Head", &bAimHead);

	ImGui::Spacing();
	ImGui::SeparatorText("Prediction");
	ImGui::Checkbox("Enable Prediction", &bPrediction);
	if (bPrediction)
	{
		ImGui::SetNextItemWidth(150.0f);
		ImGui::InputFloat("Bullet Velocity", &fBulletVelocity, 0.0f, 0.0f, "%.0f");
	}

	ImGui::End();
}

void Aimbot::OnFrame(DMA_Connection* Conn)
{
	if (!bMasterToggle) return;
	if (c_keys::IsInitialized() == false || MyMakcu::m_Device.isConnected() == false) return;
	if (Keybinds::Aimbot.IsActive(Conn) == false) return;

	auto WindowSize = Fuser::m_ScreenSize;
	Vector2 CenterScreen{ WindowSize.x / 2.0f, WindowSize.y / 2.0f };
	auto LastTime = std::chrono::steady_clock::time_point();

	// Humanization state
	Vector2 CurrentOffset{ 0.0f, 0.0f };
	bool bIsOvershootActive = false;

	do
	{
		auto CurrentTime = std::chrono::high_resolution_clock::now();
		auto DeltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - LastTime).count();
		if (DeltaTime < 5) continue;
		LastTime = CurrentTime;

		Vector2 Delta = GetAimDelta(CenterScreen);
		float Distance = sqrtf(Delta.x * Delta.x + Delta.y * Delta.y);

		// Calculate smooth values (higher = smoother/slower)
		float smoothX = std::max(1.0f, fSmoothX);
		float smoothY = bIndependentSmoothing ? std::max(1.0f, fSmoothY) : smoothX;

		// Distance-based smoothing
		if (bHumanization && bDistanceBasedSmoothing)
		{
			float distanceFactor = std::clamp(Distance / 100.0f, 0.5f, 2.0f);
			smoothX *= distanceFactor;
			smoothY *= distanceFactor;
		}

		Vector2 MoveAmount{ Delta.x / smoothX, Delta.y / smoothY };

		// Apply humanization
		if (bHumanization)
		{
			MoveAmount = ApplyHumanization(MoveAmount, Distance, bIsOvershootActive, CurrentOffset);
		}

		MyMakcu::m_Device.mouseMove(MoveAmount.x, MoveAmount.y);

		Deadlock::UpdateViewMatrix(Conn);
		EntityList::QuickPawnRefresh(Conn, &Deadlock::Proc());
		EntityList::QuickControllerRefresh(Conn, &Deadlock::Proc());

	} while (Keybinds::Aimbot.IsActive(Conn));
}

void Aimbot::RenderFOVCircle()
{
	if (!bMasterToggle || !bDrawMaxFOV) return;

	auto WindowSize = ImGui::GetWindowSize();
	auto WindowPos = ImGui::GetWindowPos();
	ImVec2 CenterScreen{ WindowPos.x + (WindowSize.x / 2.0f), WindowPos.y + (WindowSize.y / 2.0f) };
	ImGui::GetWindowDrawList()->AddCircle(CenterScreen, fMaxPixelDistance, ColorPicker::AimbotFOVCircle, 100, 1.5f);
}

Vector2 Aimbot::GetAimDelta(const Vector2& CenterScreen)
{
	std::scoped_lock lk(EntityList::m_PawnMutex, EntityList::m_ControllerMutex);

	Vector2 BestTargetDelta{};
	float BestDistance = FLT_MAX;

	for (auto& Pawn : EntityList::m_PlayerPawns)
	{
		if (Pawn.IsInvalid() || Pawn.IsLocalPlayer() || Pawn.IsFriendly())
			continue;

		auto ControllerAddress = EntityList::GetEntityAddressFromHandle(Pawn.m_hController);

		if (!ControllerAddress)
			continue;

		auto ControllerIt = std::ranges::find(EntityList::m_PlayerControllers, CBaseEntity{ ControllerAddress });

		if (ControllerIt == EntityList::m_PlayerControllers.end())
			continue;

		if (ControllerIt->IsDead())
			continue;

		auto& HeroId = ControllerIt->m_HeroID;

		auto AimList = Aimpoints::GetAimpoints(HeroId);
		size_t FinalAimpointIndex = (bAimHead) ? AimList.first : AimList.second;

		Vector3 AimPointWorldPos = Pawn.m_BonePositions[FinalAimpointIndex];

		float DistanceFromOrigin = Pawn.DistanceFromLocalPlayer(false);

		/* magic velocity formula; WIP */
		if (bPrediction)
		{
			float VelocityMultiplier{ (DistanceFromOrigin / fBulletVelocity) * 0.02479975f };
			AimPointWorldPos += (Pawn.m_Velocity * VelocityMultiplier);
		}

		Vector2 ScreenPos{};
		if (!Deadlock::WorldToScreen(AimPointWorldPos, ScreenPos)) continue;

		Vector2 Delta = ScreenPos - CenterScreen;
		float Distance = sqrtf(Delta.x * Delta.x + Delta.y * Delta.y);

		if (Distance > fMaxPixelDistance) continue;

		if (Distance < BestDistance)
		{
			BestDistance = Distance;
			BestTargetDelta = Delta;
		}
	}

	return BestTargetDelta;
}

Vector2 Aimbot::ApplyHumanization(Vector2 MoveAmount, float Distance, bool& bOvershootActive, Vector2& CurrentOffset)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> randomDist(-1.0f, 1.0f);
	std::uniform_real_distribution<float> chanceDist(0.0f, 1.0f);

	// Add random micro-movements
	if (fRandomness > 0.0f)
	{
		float randomX = randomDist(gen) * fRandomness * 0.1f;
		float randomY = randomDist(gen) * fRandomness * 0.1f;
		MoveAmount.x += randomX;
		MoveAmount.y += randomY;
	}

	// Micro-corrections (small adjustments as we get closer)
	if (fMicroCorrection > 0.0f && Distance < 50.0f)
	{
		float microX = randomDist(gen) * fMicroCorrection * 0.05f;
		float microY = randomDist(gen) * fMicroCorrection * 0.05f;
		MoveAmount.x += microX;
		MoveAmount.y += microY;
	}

	// Overshoot effect (occasionally move slightly past target)
	if (!bOvershootActive && Distance > 20.0f && chanceDist(gen) < fOvershootChance * 0.01f)
	{
		bOvershootActive = true;
		CurrentOffset.x = MoveAmount.x * (fOvershootAmount / 100.0f);
		CurrentOffset.y = MoveAmount.y * (fOvershootAmount / 100.0f);
	}

	if (bOvershootActive)
	{
		MoveAmount.x += CurrentOffset.x;
		MoveAmount.y += CurrentOffset.y;

		// Gradually reduce overshoot
		CurrentOffset.x *= 0.9f;
		CurrentOffset.y *= 0.9f;

		if (std::abs(CurrentOffset.x) < 0.1f && std::abs(CurrentOffset.y) < 0.1f)
		{
			bOvershootActive = false;
			CurrentOffset = Vector2{ 0.0f, 0.0f };
		}
	}

	return MoveAmount;
}
