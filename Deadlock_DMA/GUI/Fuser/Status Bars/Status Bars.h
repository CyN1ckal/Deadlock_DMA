#pragma once

struct GameStatistics
{
public:
	uint32_t m_FriendlyTeamHealth{ 0 };
	uint32_t m_EnemyTeamHealth{ 0 };
	uint32_t m_FriendlySoulsCollected{ 0 };
	uint32_t m_EnemySoulsCollected{ 0 };
	uint32_t m_FriendlyUnspentSouls{ 0 };
	uint32_t m_EnemyUnspentSouls{ 0 };

	GameStatistics();
};

class StatusBars
{
public:
	static void Render();

public:
	static inline bool bMasterToggle{ true };
	static inline bool bRenderTeamHealthBar{ true };
	static inline bool bRenderTeamSoulsBar{ true };
	static inline bool bRenderUnspentSoulsBar{ true };
	static inline float BarHeight{ 20.0f };

private:
	using ValuePair = std::pair<uint32_t, uint32_t>;
	using ColorPair = std::pair<ImColor, ImColor>;
	static void RenderGenericComparisonBar(ValuePair Values, ColorPair Colors, int& LineNumber, ImDrawList* DrawList, const ImVec2& WindowPos, const ImVec2& WindowSize);
	static void RenderTeamHealthBars(GameStatistics& Stats, const ImVec2& WindowPos, const ImVec2& WindowSize, ImDrawList* DrawList, int& LineNumber);
	static void RenderTeamSoulsBars(GameStatistics& Stats, const ImVec2& WindowPos, const ImVec2& WindowSize, ImDrawList* DrawList, int& LineNumber);
	static void RenderUnspentSoulsBar(GameStatistics& Stats, const ImVec2& WindowPos, const ImVec2& WindowSize, ImDrawList* DrawList, int& LineNumber);
};