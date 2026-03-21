#pragma once

#include <obs.h>
#include <string>
#include <vector>
#include "omni-projector-settings.hpp"

struct SourceGroups {
	std::vector<std::string> scenes;
	std::vector<std::string> sources;
};

class OmniProjectorManager {
public:
	static OmniProjectorManager &Get();

	// 核心功能
	void StartProjection(const std::string &sourceName, int monitor_id);
	void ProjectAll();
	void StopAllProjections();

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

	OmniProjectorManager(const OmniProjectorManager &) = delete;
	OmniProjectorManager &operator=(const OmniProjectorManager &) = delete;
};
