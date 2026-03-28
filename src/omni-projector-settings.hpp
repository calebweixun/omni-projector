#pragma once

#include <string>
#include <vector>

struct MappingEntry {
	std::string source_name;
	int monitor_index;
};

class OmniProjectorSettings {
public:
	static void Save(const std::vector<MappingEntry> &mappings);
	static std::vector<MappingEntry> Load();

	// Language settings
	static void SaveLanguageSettings(const std::string &languageMode, const std::string &selectedLanguage);
	static void LoadLanguageSettings(std::string &languageMode, std::string &selectedLanguage);
};
