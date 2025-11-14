#include "pch.h"

#include "Status Bars.h"

#include "Deadlock/Entity List/EntityList.h"

#include "GUI/Color Picker/Color Picker.h"

void StatusBars::RenderGenericComparisonBar(ValuePair Values, ColorPair Colors, int& LineNumber, ImDrawList* DrawList, const ImVec2& WindowPos, const ImVec2& WindowSize)
{
	constexpr float TotalWidth = 200.0f;
	constexpr float Padding = 2.0f;
	ImColor BackgroundColor = ImColor(50, 50, 50, 200);

	ImVec2 CenterPos = ImVec2(WindowPos.x + (WindowSize.x * 0.5f), WindowPos.y + (WindowSize.y * 0.5f));

	/* Draw background */
	ImVec2 TopLeft = ImVec2(CenterPos.x - (TotalWidth * 0.5f), (BarHeight * LineNumber));
	ImVec2 BottomRight = ImVec2(TopLeft.x + TotalWidth, TopLeft.y + BarHeight);
	DrawList->AddRectFilled(TopLeft, BottomRight, BackgroundColor);

	constexpr float EffectiveWidth = TotalWidth - (Padding * 2.0f);
	TopLeft.x += Padding;
	TopLeft.y += Padding;
	BottomRight.x -= Padding;
	BottomRight.y -= Padding;
	DrawList->AddRectFilled(TopLeft, BottomRight, Colors.second);

	uint32_t TotalValue = Values.first + Values.second;
	float FirstValuePercentage = static_cast<float>(Values.first) / static_cast<float>(TotalValue);

	BottomRight.x = TopLeft.x + (EffectiveWidth * FirstValuePercentage);
	DrawList->AddRectFilled(TopLeft, BottomRight, Colors.first);

	std::string ValueString = std::format("{} / {}", Values.first, Values.second);
	ImVec2 TextSize = ImGui::CalcTextSize(ValueString.c_str());
	ImVec2 TextPos = ImVec2(CenterPos.x - (TextSize.x * 0.5f), TopLeft.y);
	DrawList->AddText(TextPos, ImColor(255, 255, 255, 255), ValueString.c_str());

	LineNumber++;
}

void StatusBars::RenderTeamHealthBars(GameStatistics& Stats, const ImVec2& WindowPos, const ImVec2& WindowSize, ImDrawList* DrawList, int& LineNumber)
{

	RenderGenericComparisonBar(
		ValuePair(Stats.m_FriendlyTeamHealth, Stats.m_EnemyTeamHealth),
		ColorPair(ColorPicker::FriendlyHealthStatusBarColor, ColorPicker::EnemyHealthStatusBarColor),
		LineNumber,
		DrawList,
		WindowPos,
		WindowSize
	);
}

void StatusBars::RenderTeamSoulsBars(GameStatistics& Stats, const ImVec2& WindowPos, const ImVec2& WindowSize, ImDrawList* DrawList, int& LineNumber)
{
	RenderGenericComparisonBar(
		ValuePair(Stats.m_FriendlySoulsCollected, Stats.m_EnemySoulsCollected),
		ColorPair(ColorPicker::FriendlySoulsStatusBarColor, ColorPicker::EnemySoulsStatusBarColor),
		LineNumber,
		DrawList,
		WindowPos,
		WindowSize
	);
}

void StatusBars::RenderUnspentSoulsBar(GameStatistics& Stats, const ImVec2& WindowPos, const ImVec2& WindowSize, ImDrawList* DrawList, int& LineNumber)
{
	RenderGenericComparisonBar(
		ValuePair(Stats.m_FriendlyUnspentSouls, Stats.m_EnemyUnspentSouls),
		ColorPair(ColorPicker::FriendlySoulsStatusBarColor, ColorPicker::EnemySoulsStatusBarColor),
		LineNumber,
		DrawList,
		WindowPos,
		WindowSize
	);
}

void StatusBars::Render()
{
	ZoneScoped;

	auto WindowPos = ImGui::GetWindowPos();
	auto DrawList = ImGui::GetWindowDrawList();
	auto WindowSize = ImGui::GetWindowSize();

	GameStatistics Stats;

	int LineNumber = 3;

	if (bRenderTeamHealthBar)
		RenderTeamHealthBars(Stats, WindowPos, WindowSize, DrawList, LineNumber);

	if (bRenderTeamSoulsBar)
		RenderTeamSoulsBars(Stats, WindowPos, WindowSize, DrawList, LineNumber);

	if (bRenderUnspentSoulsBar)
		RenderUnspentSoulsBar(Stats, WindowPos, WindowSize, DrawList, LineNumber);
}

GameStatistics::GameStatistics()
{
	std::scoped_lock Lock(EntityList::m_ControllerMutex, EntityList::m_PawnMutex);

	for (auto& Controller : EntityList::m_PlayerControllers)
	{
		if (Controller.IsInvalid()) continue;

		auto AssociatedPawnAddr = EntityList::GetEntityAddressFromHandle(Controller.m_hPawn);

		if (!AssociatedPawnAddr) continue;

		auto PawnIt = std::find(EntityList::m_PlayerPawns.begin(), EntityList::m_PlayerPawns.end(), AssociatedPawnAddr);

		if (PawnIt == EntityList::m_PlayerPawns.end())
			continue;

		auto& Pawn = *PawnIt;

		if (Controller.IsFriendly())
		{
			if (Controller.m_CurrentHealth > 0)
				m_FriendlyTeamHealth += Controller.m_CurrentHealth;
			m_FriendlySoulsCollected += Controller.m_TotalSouls;
			m_FriendlyUnspentSouls += Pawn.m_TotalUnspentSouls;
		}
		else
		{
			if (Controller.m_CurrentHealth > 0)
				m_EnemyTeamHealth += Controller.m_CurrentHealth;
			m_EnemySoulsCollected += Controller.m_TotalSouls;
			m_EnemyUnspentSouls += Pawn.m_TotalUnspentSouls;
		}
	}
}