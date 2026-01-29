#include "pch.h"

#include "Config.h"

#include "GUI/Aimbot/Aimbot.h"

#include "GUI/Fuser/Fuser.h"

#include "GUI/Fuser/ESP/ESP.h"

#include "GUI/Fuser/ESP/Draw/Camps.h"
#include "GUI/Fuser/ESP/Draw/Players.h"
#include "GUI/Fuser/ESP/Draw/Sinners.h"
#include "GUI/Fuser/ESP/Draw/Troopers.h"

#include "GUI/Fuser/Status Bars/Status Bars.h"

#include "GUI/Keybinds/Keybinds.h"

#include "GUI/Radar/Radar.h"

#include "GUI/Color Picker/Color Picker.h"

#include "GUI/Main Menu/Main Menu.h"

#include <shlobj.h>
#include <fstream>

std::string Config::getConfigDir() {
	char path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(nullptr, CSIDL_PERSONAL, nullptr, 0, path))) {
		std::filesystem::path docPath(path);
		docPath /= "DEADLOCK-DMA";
		docPath /= "Configs";
		if (!std::filesystem::exists(docPath)) {
			std::filesystem::create_directories(docPath);
		}
		return docPath.string();
	}

	std::filesystem::path fallback("Configs");
	if (!std::filesystem::exists(fallback))
		std::filesystem::create_directory(fallback);
	return fallback.string();
}

std::string Config::getConfigPath(const std::string& configName) {
	std::filesystem::path p = getConfigDir();
	p /= configName + ".json";
	return p.string();
}

void Config::RefreshConfigFilesList(std::vector<std::string>& outList) {
	outList.clear();
	try {
		for (const auto& entry : std::filesystem::directory_iterator(getConfigDir())) {
			if (entry.is_regular_file() && entry.path().extension() == ".json") {
				outList.push_back(entry.path().stem().string());
			}
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::println("[Config] Error listing config files: {}", e.what());
	}
}

void Config::Render()
{
	ImGui::Begin("Configuration Manager");

	static char configNameBuf[128] = "default";
	static int selectedConfig = -1;
	static std::vector<std::string> configFiles;
	static bool bFirstRun = true;

	if (bFirstRun)
	{
		RefreshConfigFilesList(configFiles);
		bFirstRun = false;
	}

	ImGui::BeginChild("##ConfigActions", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 0), true);

	ImGui::SeparatorText("Config Name");
	ImGui::SetNextItemWidth(-1);
	ImGui::InputTextWithHint("##ConfigName", "Enter config name...", configNameBuf, IM_ARRAYSIZE(configNameBuf));

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// Save/Load/Delete buttons
	ImGui::SeparatorText("Actions");

	ImVec2 buttonSize(ImGui::GetContentRegionAvail().x, 0);

	if (ImGui::Button("Save Config", buttonSize))
	{
		if (strlen(configNameBuf) > 0)
		{
			SaveConfig(configNameBuf);
			RefreshConfigFilesList(configFiles);
		}
	}

	if (ImGui::Button("Load Config", buttonSize))
	{
		if (strlen(configNameBuf) > 0)
		{
			LoadConfig(configNameBuf);
		}
	}

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.1f, 0.1f, 1.0f));

	bool canDelete = selectedConfig >= 0 && selectedConfig < static_cast<int>(configFiles.size());
	if (!canDelete) ImGui::BeginDisabled();

	if (ImGui::Button("Delete Config", buttonSize))
	{
		std::string fileToDelete = Config::getConfigPath(configFiles[selectedConfig]);
		if (std::filesystem::exists(fileToDelete))
		{
			std::error_code ec;
			std::filesystem::remove(fileToDelete, ec);
			if (!ec)
			{
				selectedConfig = -1;
				strncpy_s(configNameBuf, sizeof(configNameBuf), "default", _TRUNCATE);
				RefreshConfigFilesList(configFiles);
			}
		}
	}

	if (!canDelete) ImGui::EndDisabled();
	ImGui::PopStyleColor(3);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// Utilities
	ImGui::SeparatorText("Utilities");

	if (ImGui::Button("Open Config Folder", buttonSize))
	{
		ShellExecuteA(nullptr, "open", getConfigDir().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
	}

	if (ImGui::Button("Refresh List", buttonSize))
	{
		RefreshConfigFilesList(configFiles);
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// Info
	ImGui::SeparatorText("Info");
	ImGui::TextDisabled("Total configs: %zu", configFiles.size());
	if (selectedConfig >= 0)
	{
		ImGui::TextDisabled("Selected: %s", configFiles[selectedConfig].c_str());
	}

	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("##ConfigList", ImVec2(0, 0), true);

	ImGui::SeparatorText("Available Configs");

	if (!configFiles.empty()) {
		for (size_t i = 0; i < configFiles.size(); ++i)
		{
			bool isSelected = (selectedConfig == static_cast<int>(i));

			if (ImGui::Selectable(configFiles[i].c_str(), isSelected))
			{
				selectedConfig = static_cast<int>(i);
				strncpy_s(configNameBuf, sizeof(configNameBuf), configFiles[i].c_str(), _TRUNCATE);
			}

			// Double-click to load
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{
				LoadConfig(configFiles[i]);
			}
		}
	}

	ImGui::EndChild();

	ImGui::End();
}

static void DeserializeKeybindObj(const json& Table, const std::string& Name, CKeybind& Out)
{
	if (!Table.contains(Name))
		return;

	const auto& k = Table[Name];
	if (k.contains("m_Key")) Out.m_Key = k["m_Key"].get<uint32_t>();
	if (k.contains("m_bTargetPC")) Out.m_bTargetPC = k["m_bTargetPC"].get<bool>();
	if (k.contains("m_bRadarPC")) Out.m_bRadarPC = k["m_bRadarPC"].get<bool>();
}

void Config::DeserializeKeybinds(const json& Table) {
	if (Table.contains("bSettings")) {
		Aimbot::bSettings = Table["bSettings"].get<bool>();
	}
	DeserializeKeybindObj(Table, "Debug", Keybinds::Debug);
	DeserializeKeybindObj(Table, "Aimbot", Keybinds::Aimbot);
}

static json SerializeKeybindEntryObj(const CKeybind& kb) {
	return json{
		{ "m_Key", kb.m_Key },
		{ "m_bTargetPC", kb.m_bTargetPC },
		{ "m_bRadarPC", kb.m_bRadarPC }
	};
}

json Config::SerializeKeybinds(json& j) {
	j["Keybinds"] = {
		{ "bSettings", Keybinds::bSettings },
		{ "Debug", SerializeKeybindEntryObj(Keybinds::Debug) },
		{ "Aimbot", SerializeKeybindEntryObj(Keybinds::Aimbot) }
	};
	return j;
}

json Config::SerializeConfig() {
	json j;

	j["MainMenu"] = {
		{"bVSync", MainMenu::bVSync},
		{"iTargetFPS", MainMenu::iTargetFPS},
	};

	// Aimbot
	j["Aimbot"] = {
		// General
		{"bSettings", Aimbot::bSettings},

		// Targetting
		{"fSmoothX", Aimbot::fSmoothX},
		{"fSmoothY", Aimbot::fSmoothY},
		{"fGaussianNoise", Aimbot::fGaussianNoise},
		{"fMaxPixelDistance", Aimbot::fMaxPixelDistance},
		{"bAimHead", Aimbot::bAimHead},
		{"bDrawMaxFOV", Aimbot::bDrawMaxFOV},

		// Prediction
		{"bPrediction", Aimbot::bPrediction},
		{"fBulletVelocity", Aimbot::fBulletVelocity}
	};

	j["Fuser"] = {
		{"ScreenSize", {Fuser::m_ScreenSize.x, Fuser::m_ScreenSize.y}},
		{"bDrawSoulsPerMinute", Fuser::bDrawSoulsPerMinute},
		{"bMasterToggle", Fuser::bMasterToggle},

		{"ESP", {
			{"bMasterToggle", ESP::bMasterToggle}
		}},

		{"Draw_Camps", {
			{"bMasterToggle", Draw_Camps::bMasterToggle}
		}},

		{"Draw_Players", {
			{"bMasterToggle", Draw_Players::bMasterToggle},
			{"bHideFriendly", Draw_Players::bHideFriendly},
			{"bDrawBones", Draw_Players::bDrawBones},
			{"fBonesThickness", Draw_Players::fBonesThickness},
			{"bDrawHead", Draw_Players::bDrawHead},
			{"bDrawVelocityVector", Draw_Players::bDrawVelocityVector},
			{"bDrawUnsecuredSouls", Draw_Players::bDrawUnsecuredSouls},
			{"UnsecuredSoulsMinimumThreshold", Draw_Players::UnsecuredSoulsMinimumThreshold},
			{"UnsecuredSoulsHighlightThreshold", Draw_Players::UnsecuredSoulsHighlightThreshold},
			{"bBoneNumbers", Draw_Players::bBoneNumbers},
			{"bDrawHealthBar", Draw_Players::bDrawHealthBar},
			{"bHideLocalPlayer", Draw_Players::bHideLocalPlayer},
			{"bShowLevel", Draw_Players::bShowLevel},
			{"bShowDistance", Draw_Players::bShowDistance}
		}},

		{"Draw_Sinners", {
			{"bMasterToggle", Draw_Sinners::bMasterToggle}
		}},

		{"Draw_Troopers", {
			{"bMasterToggle", Draw_Troopers::bMasterToggle},
			{"bHideFriendly", Draw_Troopers::bHideFriendly}
		}},

		{"StatusBars", {
			{"bMasterToggle", StatusBars::bMasterToggle},
			{"bRenderTeamHealthBar", StatusBars::bRenderTeamHealthBar},
			{"bRenderTeamSoulsBar", StatusBars::bRenderTeamSoulsBar},
			{"bRenderUnspentSoulsBar", StatusBars::bRenderUnspentSoulsBar},
			{"BarHeight", StatusBars::BarHeight}
		}}
	};

	j["Radar"] = {
		{"bMasterToggle", Radar::bMasterToggle},
		{"bSettings", Radar::bSettings},
		{"bHideFriendly", Radar::bHideFriendly},
		{"bMobaStyle", Radar::bMobaStyle},
		{"fRadarScale", Radar::fRadarScale},
		{"fRaySize", Radar::fRaySize},
		{"fRadarPadding", Radar::fRadarPadding}
	};

	j["ColorPicker"] = {
		{"FriendlyNameTagColor", static_cast<uint32_t>(ColorPicker::FriendlyNameTagColor)},
		{"EnemyNameTagColor", static_cast<uint32_t>(ColorPicker::EnemyNameTagColor)},
		{"EnemyBoneColor", static_cast<uint32_t>(ColorPicker::EnemyBoneColor)},
		{"FriendlyBoneColor", static_cast<uint32_t>(ColorPicker::FriendlyBoneColor)},
		{"SinnersColor", static_cast<uint32_t>(ColorPicker::SinnersColor)},
		{"MonsterCampColor", static_cast<uint32_t>(ColorPicker::MonsterCampColor)},
		{"FriendlyRadarColor", static_cast<uint32_t>(ColorPicker::FriendlyRadarColor)},
		{"EnemyRadarColor", static_cast<uint32_t>(ColorPicker::EnemyRadarColor)},
		{"FriendlyTrooperColor", static_cast<uint32_t>(ColorPicker::FriendlyTrooperColor)},
		{"EnemyTrooperColor", static_cast<uint32_t>(ColorPicker::EnemyTrooperColor)},
		{"UnsecuredSoulsTextColor", static_cast<uint32_t>(ColorPicker::UnsecuredSoulsTextColor)},
		{"UnsecuredSoulsHighlightedTextColor", static_cast<uint32_t>(ColorPicker::UnsecuredSoulsHighlightedTextColor)},
		{"FriendlyHealthStatusBarColor", static_cast<uint32_t>(ColorPicker::FriendlyHealthStatusBarColor)},
		{"EnemyHealthStatusBarColor", static_cast<uint32_t>(ColorPicker::EnemyHealthStatusBarColor)},
		{"FriendlySoulsStatusBarColor", static_cast<uint32_t>(ColorPicker::FriendlySoulsStatusBarColor)},
		{"EnemySoulsStatusBarColor", static_cast<uint32_t>(ColorPicker::EnemySoulsStatusBarColor)},
		{"HealthBarForegroundColor", static_cast<uint32_t>(ColorPicker::HealthBarForegroundColor)},
		{"HealthBarBackgroundColor", static_cast<uint32_t>(ColorPicker::HealthBarBackgroundColor)},
		{"AimbotFOVCircle", static_cast<uint32_t>(ColorPicker::AimbotFOVCircle)},
		{"RadarBackgroundColor", static_cast<uint32_t>(ColorPicker::RadarBackgroundColor)}
	};

	SerializeKeybinds(j);

	return j;
}

void Config::DeserializeConfig(const json& j) {

	// MainMenu
	if (j.contains("MainMenu")) {
		const auto m = j["MainMenu"];

		if (m.contains("bVSync")) MainMenu::bVSync = m["bVSync"].get<bool>();
		if (m.contains("iTargetFPS")) MainMenu::iTargetFPS = m["iTargetFPS"].get<int>();
	}

	// Aimbot
	if (j.contains("Aimbot")) {
		const auto& ab = j["Aimbot"];

		// General
		if (ab.contains("bSettings")) Aimbot::bSettings = ab["bSettings"].get<bool>();

		// Targeting
		if (ab.contains("fSmoothX")) Aimbot::fSmoothX = ab["fSmoothX"].get<float>();
		if (ab.contains("fSmoothY")) Aimbot::fSmoothY = ab["fSmoothY"].get<float>();
		if (ab.contains("fGaussianNoise")) Aimbot::fGaussianNoise = ab["fGaussianNoise"].get<float>();
		if (ab.contains("fMaxPixelDistance")) Aimbot::fMaxPixelDistance = ab["fMaxPixelDistance"].get<float>();
		if (ab.contains("bAimHead")) Aimbot::bAimHead = ab["bAimHead"].get<bool>();
		if (ab.contains("bDrawMaxFOV")) Aimbot::bDrawMaxFOV = ab["bDrawMaxFOV"].get<bool>();

		// Prediction
		if (ab.contains("bPrediction")) Aimbot::bPrediction = ab["bPrediction"].get<bool>();
		if (ab.contains("fBulletVelocity")) Aimbot::fBulletVelocity = ab["fBulletVelocity"].get<float>();
	}

	// Fuser
	if (j.contains("Fuser")) {
		const auto& fuser = j["Fuser"];

		// ScreenSize
		if (fuser.contains("ScreenSize")) {
			const auto& screenSizeJson = fuser["ScreenSize"];
			if (screenSizeJson.is_array() && screenSizeJson.size() == 2) {
				Fuser::m_ScreenSize = ImVec2(screenSizeJson[0].get<float>(), screenSizeJson[1].get<float>());
			}
		}

		if (fuser.contains("bDrawSoulsPerMinute")) Fuser::bDrawSoulsPerMinute = fuser["bDrawSoulsPerMinute"].get<bool>();
		if (fuser.contains("bMasterToggle")) Fuser::bMasterToggle = fuser["bMasterToggle"].get<bool>();

		// ESP
		if (fuser.contains("ESP")) {
			const auto& esp = fuser["ESP"];
			if (esp.contains("bMasterToggle")) ESP::bMasterToggle = esp["bMasterToggle"].get<bool>();
		}

		// Draw_Camps
		if (fuser.contains("Draw_Camps")) {
			const auto& camps = fuser["Draw_Camps"];
			if (camps.contains("bMasterToggle")) Draw_Camps::bMasterToggle = camps["bMasterToggle"].get<bool>();
		}

		// Draw_Players
		if (fuser.contains("Draw_Players")) {
			const auto& players = fuser["Draw_Players"];
			if (players.contains("bMasterToggle")) Draw_Players::bMasterToggle = players["bMasterToggle"].get<bool>();
			if (players.contains("bHideFriendly")) Draw_Players::bHideFriendly = players["bHideFriendly"].get<bool>();
			if (players.contains("bDrawBones")) Draw_Players::bDrawBones = players["bDrawBones"].get<bool>();
			if (players.contains("fBonesThickness")) Draw_Players::fBonesThickness = players["fBonesThickness"].get<float>();
			if (players.contains("bDrawHead")) Draw_Players::bDrawHead = players["bDrawHead"].get<bool>();
			if (players.contains("bDrawVelocityVector")) Draw_Players::bDrawVelocityVector = players["bDrawVelocityVector"].get<bool>();
			if (players.contains("bDrawUnsecuredSouls")) Draw_Players::bDrawUnsecuredSouls = players["bDrawUnsecuredSouls"].get<bool>();
			if (players.contains("UnsecuredSoulsMinimumThreshold")) Draw_Players::UnsecuredSoulsMinimumThreshold = players["UnsecuredSoulsMinimumThreshold"].get<int>();
			if (players.contains("UnsecuredSoulsHighlightThreshold")) Draw_Players::UnsecuredSoulsHighlightThreshold = players["UnsecuredSoulsHighlightThreshold"].get<int>();
			if (players.contains("bBoneNumbers")) Draw_Players::bBoneNumbers = players["bBoneNumbers"].get<bool>();
			if (players.contains("bDrawHealthBar")) Draw_Players::bDrawHealthBar = players["bDrawHealthBar"].get<bool>();
			if (players.contains("bHideLocalPlayer")) Draw_Players::bHideLocalPlayer = players["bHideLocalPlayer"].get<bool>();
			if (players.contains("bShowLevel")) Draw_Players::bShowLevel = players["bShowLevel"].get<bool>();
			if (players.contains("bShowDistance")) Draw_Players::bShowDistance = players["bShowDistance"].get<bool>();
		}

		// Draw_Sinners
		if (fuser.contains("Draw_Sinners")) {
			const auto& sinners = fuser["Draw_Sinners"];
			if (sinners.contains("bMasterToggle")) Draw_Sinners::bMasterToggle = sinners["bMasterToggle"].get<bool>();
		}

		// Draw_Troopers
		if (fuser.contains("Draw_Troopers")) {
			const auto& troopers = fuser["Draw_Troopers"];
			if (troopers.contains("bMasterToggle")) Draw_Troopers::bMasterToggle = troopers["bMasterToggle"].get<bool>();
			if (troopers.contains("bHideFriendly")) Draw_Troopers::bHideFriendly = troopers["bHideFriendly"].get<bool>();
		}

		// StatusBars
		if (fuser.contains("StatusBars")) {
			const auto& bars = fuser["StatusBars"];
			if (bars.contains("bMasterToggle")) StatusBars::bMasterToggle = bars["bMasterToggle"].get<bool>();
			if (bars.contains("bRenderTeamHealthBar")) StatusBars::bRenderTeamHealthBar = bars["bRenderTeamHealthBar"].get<bool>();
			if (bars.contains("bRenderTeamSoulsBar")) StatusBars::bRenderTeamSoulsBar = bars["bRenderTeamSoulsBar"].get<bool>();
			if (bars.contains("bRenderUnspentSoulsBar")) StatusBars::bRenderUnspentSoulsBar = bars["bRenderUnspentSoulsBar"].get<bool>();
			if (bars.contains("BarHeight")) StatusBars::BarHeight = bars["BarHeight"].get<float>();
		}
	}

	// Radar
	if (j.contains("Radar")) {
		const auto& radar = j["Radar"];
		if (radar.contains("bMasterToggle")) Radar::bMasterToggle = radar["bMasterToggle"].get<bool>();
		if (radar.contains("bSettings")) Radar::bSettings = radar["bSettings"].get<bool>();
		if (radar.contains("bHideFriendly")) Radar::bHideFriendly = radar["bHideFriendly"].get<bool>();
		if (radar.contains("bMobaStyle")) Radar::bMobaStyle = radar["bMobaStyle"].get<bool>();
		if (radar.contains("fRadarScale")) Radar::fRadarScale = radar["fRadarScale"].get<float>();
		if (radar.contains("fRaySize")) Radar::fRaySize = radar["fRaySize"].get<float>();
		if (radar.contains("fRadarPadding")) Radar::fRadarPadding = radar["fRadarPadding"].get<float>();
	}

	// ColorPicker
	if (j.contains("ColorPicker")) {
		const auto& colors = j["ColorPicker"];
		if (colors.contains("FriendlyNameTagColor")) ColorPicker::FriendlyNameTagColor = colors["FriendlyNameTagColor"].get<uint32_t>();
		if (colors.contains("EnemyNameTagColor")) ColorPicker::EnemyNameTagColor = colors["EnemyNameTagColor"].get<uint32_t>();
		if (colors.contains("EnemyBoneColor")) ColorPicker::EnemyBoneColor = colors["EnemyBoneColor"].get<uint32_t>();
		if (colors.contains("FriendlyBoneColor")) ColorPicker::FriendlyBoneColor = colors["FriendlyBoneColor"].get<uint32_t>();
		if (colors.contains("SinnersColor")) ColorPicker::SinnersColor = colors["SinnersColor"].get<uint32_t>();
		if (colors.contains("MonsterCampColor")) ColorPicker::MonsterCampColor = colors["MonsterCampColor"].get<uint32_t>();
		if (colors.contains("FriendlyRadarColor")) ColorPicker::FriendlyRadarColor = colors["FriendlyRadarColor"].get<uint32_t>();
		if (colors.contains("EnemyRadarColor")) ColorPicker::EnemyRadarColor = colors["EnemyRadarColor"].get<uint32_t>();
		if (colors.contains("FriendlyTrooperColor")) ColorPicker::FriendlyTrooperColor = colors["FriendlyTrooperColor"].get<uint32_t>();
		if (colors.contains("EnemyTrooperColor")) ColorPicker::EnemyTrooperColor = colors["EnemyTrooperColor"].get<uint32_t>();
		if (colors.contains("UnsecuredSoulsTextColor")) ColorPicker::UnsecuredSoulsTextColor = colors["UnsecuredSoulsTextColor"].get<uint32_t>();
		if (colors.contains("UnsecuredSoulsHighlightedTextColor")) ColorPicker::UnsecuredSoulsHighlightedTextColor = colors["UnsecuredSoulsHighlightedTextColor"].get<uint32_t>();
		if (colors.contains("FriendlyHealthStatusBarColor")) ColorPicker::FriendlyHealthStatusBarColor = colors["FriendlyHealthStatusBarColor"].get<uint32_t>();
		if (colors.contains("EnemyHealthStatusBarColor")) ColorPicker::EnemyHealthStatusBarColor = colors["EnemyHealthStatusBarColor"].get<uint32_t>();
		if (colors.contains("FriendlySoulsStatusBarColor")) ColorPicker::FriendlySoulsStatusBarColor = colors["FriendlySoulsStatusBarColor"].get<uint32_t>();
		if (colors.contains("EnemySoulsStatusBarColor")) ColorPicker::EnemySoulsStatusBarColor = colors["EnemySoulsStatusBarColor"].get<uint32_t>();
		if (colors.contains("HealthBarForegroundColor")) ColorPicker::HealthBarForegroundColor = colors["HealthBarForegroundColor"].get<uint32_t>();
		if (colors.contains("HealthBarBackgroundColor")) ColorPicker::HealthBarBackgroundColor = colors["HealthBarBackgroundColor"].get<uint32_t>();
		if (colors.contains("AimbotFOVCircle")) ColorPicker::AimbotFOVCircle = colors["AimbotFOVCircle"].get<uint32_t>();
		if (colors.contains("AimbotFOVCircleActive")) ColorPicker::AimbotFOVCircleActive = colors["AimbotFOVCircleActive"].get<uint32_t>();
		if (colors.contains("RadarBackgroundColor")) ColorPicker::RadarBackgroundColor = colors["RadarBackgroundColor"].get<uint32_t>();
	}

	// Keybinds
	if (j.contains("Keybinds")) {
		DeserializeKeybinds(j["Keybinds"]);
	}
}

void Config::SaveConfig(const std::string& configName) {
	std::println("[Config] Saving config: {}", configName);
	json j = SerializeConfig();
	std::ofstream file(getConfigPath(configName));
	if (!file.is_open())
		return;
	file << std::setw(4) << j;
	file.close();
}

bool Config::LoadConfig(const std::string& configName) {
	std::println("[Config] Loading config: {}", configName);
	std::ifstream file(getConfigPath(configName));
	if (!file.is_open())
	{
		std::println("[Config] Failed to open config file: {}", getConfigPath(configName));
		return false;
	}
	json j;
	try {
		file >> j;
	}
	catch (...) {
		file.close();
		return false;
	}
	file.close();
	DeserializeConfig(j);
	return true;
}