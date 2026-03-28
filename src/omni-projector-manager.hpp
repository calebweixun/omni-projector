#pragma once

#include <obs.h>
#include <string>
#include <vector>
#include "omni-projector-settings.hpp"
#include <QTimer>
#include <QLocale>

struct SourceGroups {
	std::vector<std::string> scenes;
	std::vector<std::string> sources;
};

class OmniProjectorManager {
public:
	static OmniProjectorManager &Get();
	// language control
	void SetLanguageMode(const std::string &mode);
	void SetSelectedLanguage(const std::string &lang);
	void ApplyLocalization();

	// 核心功能
	void StartProjection(const std::string &sourceName, int monitor_id);
	void ProjectAll();
	void StopAllProjections();
	void StopProjectionByName(const std::string &sourceName);

	// 配置管理
	void AddMapping(const std::string &source, int monitor);
	void UpdateMapping(int index, const std::string &source, int monitor);
	void RemoveMapping(int index);
	const std::vector<MappingEntry> &GetMappings() const { return mappings; }
	void SaveSettings();
	void LoadSettings();

	// 資料取得
	SourceGroups GetAvailableSources();
	int GetMonitorCount();

private:
	OmniProjectorManager();
	~OmniProjectorManager() = default;

	std::vector<MappingEntry> mappings;
	// language settings
	std::string languageMode; // "follow_obs" or "override"
	std::string selectedLanguage; // ISO code when override
	QTimer *localeTimer = nullptr;
	std::string lastSystemLocale;

	OmniProjectorManager(const OmniProjectorManager &) = delete;
	OmniProjectorManager &operator=(const OmniProjectorManager &) = delete;
};
