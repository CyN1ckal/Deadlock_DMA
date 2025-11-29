#include "pch.h"
#include "DMA/DMA.h"
#include "DMA/Input Manager.h"
#include "GUI/Keybinds/Keybinds.h"
#include "Aimbot.h"
#include "GUI/Fuser/Fuser.h"
#include "GUI/Color Picker/Color Picker.h"

void Aimbot::RenderSettings()
{
	static bool bFirstFrame{ true };
	if (bFirstFrame)
	{
		m_Device.connect();
		bFirstFrame = false;
	}

	ImGui::Begin("Aimbot");

	if (m_Device.isConnected())
		ImGui::TextColored(ImColor(0, 255, 0), "Makcu Connected!");
	else
		ImGui::TextColored(ImColor(255, 0, 0), "Makcu Disconnected!");

	ImGui::SliderFloat("Dampen", &fDampen, 0.1f, 1.0f, "%.2f");

	ImGui::SliderFloat("Max Pixel Distance", &fMaxPixelDistance, 10.0f, 500.0f, "%.1f");

	ImGui::InputFloat("Bullet Velocity", &fBulletVelocity);

	ImGui::Checkbox("Prediction", &bPrediction);

	ImGui::Checkbox("Aim Head?", &bAimHead);

	ImGui::SameLine();

	ImGui::Checkbox("FOV Circle", &bDrawMaxFOV);

	ImGui::End();
}


#include "Deadlock/Entity List/EntityList.h"
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

void Aimbot::OnFrame(DMA_Connection* Conn)
{
	if (m_Device.isConnected() == false) return;

	auto WindowSize = Fuser::ScreenSize;
	Vector2 CenterScreen{ WindowSize.x / 2.0f, WindowSize.y / 2.0f };

	std::println("Aimbot::OnFrame called");

	auto LastTime = std::chrono::steady_clock::time_point();

	do
	{
		auto CurrentTime = std::chrono::high_resolution_clock::now();

		auto DeltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - LastTime).count();

		if (DeltaTime < 5) continue;

		LastTime = CurrentTime;

		Vector2 Delta = GetAimDelta(CenterScreen);

		Vector2 MoveAmount{ Delta.x * fDampen, Delta.y * fDampen };

		m_Device.mouseMove(MoveAmount.x, MoveAmount.y);

		Deadlock::UpdateViewMatrix(Conn);
		EntityList::QuickPawnRefresh(Conn, &Deadlock::Proc());
		EntityList::QuickControllerRefresh(Conn, &Deadlock::Proc());

	} while (c_keys::IsKeyDown(Conn, Keybinds::m_AimbotHotkey));
}

void Aimbot::RenderFOVCircle()
{
	if (!bDrawMaxFOV)
		return;

	auto WindowSize = ImGui::GetWindowSize();
	auto WindowPos = ImGui::GetWindowPos();
	ImVec2 CenterScreen{ WindowPos.x + (WindowSize.x / 2.0f), WindowPos.y + (WindowSize.y / 2.0f) };
	ImGui::GetWindowDrawList()->AddCircle(CenterScreen, fMaxPixelDistance, ColorPicker::AimbotFOVCircle, 100, 1.5f);
}