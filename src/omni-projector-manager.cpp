#include "omni-projector-manager.hpp"
#include <obs-frontend-api.h>
#include <QApplication>
#include <QWidget>

OmniProjectorManager::OmniProjectorManager()
{
	LoadSettings();
}

OmniProjectorManager &OmniProjectorManager::Get()
{
	static OmniProjectorManager instance;
	return instance;
}

void OmniProjectorManager::StartProjection(obs_source_t *source, int monitor_id)
{
	if (!source)
		return;
	const char *name = obs_source_get_name(source);
	obs_frontend_open_projector("Source", monitor_id, nullptr, name);
}

void OmniProjectorManager::ProjectAll()
{
	for (const auto &entry : mappings) {
		obs_source_t *source = obs_get_source_by_name(entry.source_name.c_str());
		if (source) {
			StartProjection(source, entry.monitor_index);
			obs_source_release(source);
		}
	}
}

void OmniProjectorManager::StopAllProjections()
{
	// 透過 Qt 列舉所有頂層視窗，找出標題包含 "Projector" 的視窗並關閉
	for (QWidget *widget : QApplication::topLevelWidgets()) {
		if (widget->isWindow() && widget->windowTitle().contains("Projector")) {
			widget->close();
		}
	}
}

void OmniProjectorManager::AddMapping(const std::string &source, int monitor)
{
	mappings.push_back({source, monitor});
	SaveSettings();
}

void OmniProjectorManager::UpdateMapping(int index, const std::string &source, int monitor)
{
	if (index >= 0 && index < (int)mappings.size()) {
		mappings[index] = {source, monitor};
		SaveSettings();
	}
}

void OmniProjectorManager::RemoveMapping(int index)
{
	if (index >= 0 && index < (int)mappings.size()) {
		mappings.erase(mappings.begin() + index);
		SaveSettings();
	}
}

void OmniProjectorManager::SaveSettings()
{
	OmniProjectorSettings::Save(mappings);
}

void OmniProjectorManager::LoadSettings()
{
	mappings = OmniProjectorSettings::Load();
}

#include <algorithm>

std::vector<std::string> OmniProjectorManager::GetAvailableSources()
{
	std::vector<std::string> source_list;

	// 1. 取得一般來源 (Sources)
	auto EnumSources = [](void *data, obs_source_t *source) {
		auto *list = static_cast<std::vector<std::string> *>(data);
		uint32_t caps = obs_source_get_output_flags(source);
		if (caps & OBS_SOURCE_VIDEO) {
			list->push_back(obs_source_get_name(source));
		}
		return true;
	};
	obs_enum_sources(EnumSources, &source_list);

	// 2. 取得所有場景 (Scenes)
	struct obs_frontend_source_list scenes;
	obs_frontend_get_scenes(&scenes);
	for (size_t i = 0; i < scenes.sources.num; i++) {
		obs_source_t *scene = scenes.sources.array[i];
		std::string name = obs_source_get_name(scene);
		// 避免與 EnumSources 中重複加入
		if (std::find(source_list.begin(), source_list.end(), name) == source_list.end()) {
			source_list.push_back(name);
		}
	}
	obs_frontend_source_list_free(&scenes);

	return source_list;
}

int OmniProjectorManager::GetMonitorCount()
{
	return QGuiApplication::screens().size();
}
