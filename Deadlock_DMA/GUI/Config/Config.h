#pragma once

using json = nlohmann::json;

class Config
{
public:
	static void Render();
	static void SaveConfig(const std::string& configName);
	static bool LoadConfig(const std::string& configName);

private:
	static std::string getConfigDir();
	static std::string getConfigPath(const std::string& configName);
	static void RefreshConfigFilesList(std::vector<std::string>& outList);

private:
	static void DeserializeKeybinds(const json& Table);
	static json SerializeKeybinds(json& j);
	static json SerializeConfig();
	static void DeserializeConfig(const json& j);
};