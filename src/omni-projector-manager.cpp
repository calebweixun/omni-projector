#include "omni-projector-manager.hpp"
#include <obs-frontend-api.h>
#include <QApplication>
#include <QWidget>
#include <QTimer>
#include <QLocale>
// localization and UI
#include "localization.hpp"
#include "omni-projector-dock.hpp"

OmniProjectorManager::OmniProjectorManager()
{
	LoadSettings();
	// initialize localization based on loaded settings
	ApplyLocalization();

	// Start a timer to poll system locale as a fallback for OBS language change detection
	localeTimer = new QTimer();
	localeTimer->setInterval(2000); // 2 seconds
	lastSystemLocale = QLocale::system().name().toStdString();
	QObject::connect(localeTimer, &QTimer::timeout, [this]() {
		std::string cur = QLocale::system().name().toStdString();
		if (cur != lastSystemLocale) {
			lastSystemLocale = cur;
			// map QLocale name (zh_TW) to our file code (zh-TW)
			std::string mapped = cur;
			for (auto &c : mapped) if (c == '_') c = '-';
			// only react when following OBS (follow_obs mode)
			if (languageMode == "follow_obs") {
				selectedLanguage = mapped;
				SaveSettings();
				ApplyLocalization();
			}
		}
	});
	localeTimer->start();
}

OmniProjectorManager &OmniProjectorManager::Get()
{
	static OmniProjectorManager instance;
	return instance;
}

void OmniProjectorManager::StartProjection(const std::string &sourceName, int monitor_id)
{
	if (sourceName.empty())
		return;

	if (sourceName == "輸出" || sourceName == "輸出 (Program)") {
		obs_frontend_open_projector("StudioProgram", monitor_id, nullptr, nullptr);
	} else if (sourceName == "預覽" || sourceName == "預覽 (Preview)") {
		obs_frontend_open_projector("Preview", monitor_id, nullptr, nullptr);
	} else {
		obs_frontend_open_projector("Source", monitor_id, nullptr, sourceName.c_str());
	}
}

void OmniProjectorManager::ProjectAll()
{
	for (const auto &entry : mappings) {
		const std::string &name = entry.source_name;
		if (name == "輸出" || name == "輸出 (Program)" || name == "預覽" || name == "預覽 (Preview)") {
			StartProjection(name, entry.monitor_index);
		} else {
			obs_source_t *source = obs_get_source_by_name(name.c_str());
			if (source) {
				StartProjection(name, entry.monitor_index);
				obs_source_release(source);
			}
		}
	}
}

void OmniProjectorManager::StopAllProjections()
{
	// 透過 Qt 列舉所有頂層視窗，找出標題或類別包含與投影相關的視窗並關閉
	for (QWidget *widget : QApplication::topLevelWidgets()) {
		if (widget->isWindow()) {
			QString title = widget->windowTitle();
			if (title.contains("Projector", Qt::CaseInsensitive) || title.contains("投影") ||
			    QString(widget->metaObject()->className()).contains("OBSProjector")) {
				widget->close();
			}
		}
	}
}

void OmniProjectorManager::StopProjectionByName(const std::string &sourceName)
{
	if (sourceName.empty())
		return;

	QString target = QString::fromStdString(sourceName);
	for (QWidget *widget : QApplication::topLevelWidgets()) {
		if (widget->isWindow()) {
			QString title = widget->windowTitle();
			// 檢查視窗標題是否包含來源名稱（OBS 的投影視窗標題通常包含來源名稱）
			if (title.contains(target, Qt::CaseInsensitive) &&
			    (title.contains("Projector", Qt::CaseInsensitive) || title.contains("投影") ||
			     QString(widget->metaObject()->className()).contains("OBSProjector"))) {
				widget->close();
			}
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
		std::string name = mappings[index].source_name;
		StopProjectionByName(name);
		mappings.erase(mappings.begin() + index);
		SaveSettings();
	}
}

void OmniProjectorManager::SaveSettings()
{
	OmniProjectorSettings::Save(mappings);
	OmniProjectorSettings::SaveLanguageSettings(languageMode, selectedLanguage);
}

void OmniProjectorManager::LoadSettings()
{
	mappings = OmniProjectorSettings::Load();
	OmniProjectorSettings::LoadLanguageSettings(languageMode, selectedLanguage);
}

void OmniProjectorManager::SetLanguageMode(const std::string &mode)
{
	languageMode = mode;
	SaveSettings();
	ApplyLocalization();
}

void OmniProjectorManager::SetSelectedLanguage(const std::string &lang)
{
	selectedLanguage = lang;
	SaveSettings();
	ApplyLocalization();
}

void OmniProjectorManager::ApplyLocalization()
{
	std::string langToLoad = "en-US";
	if (languageMode == "override" && !selectedLanguage.empty()) {
		langToLoad = selectedLanguage;
	} else {
		// follow_obs behavior: TODO detect OBS language; fallback to en-US
		// For now, use selectedLanguage if present or en-US
		if (!selectedLanguage.empty())
			langToLoad = selectedLanguage;
	}

	LocalizationManager::Initialize(langToLoad);
	// Refresh UI to apply translations
	OmniProjectorDock::RefreshAll();
}

#include <algorithm>

SourceGroups OmniProjectorManager::GetAvailableSources()
{
	SourceGroups groups;

	// 1. 取得一般來源 (Sources)
	auto EnumSources = [](void *data, obs_source_t *source) {
		auto *list = static_cast<std::vector<std::string> *>(data);
		uint32_t caps = obs_source_get_output_flags(source);
		if (caps & OBS_SOURCE_VIDEO) {
			list->push_back(obs_source_get_name(source));
		}
		return true;
	};
	obs_enum_sources(EnumSources, &groups.sources);

	// 2. 取得所有場景 (Scenes)
	// 在 OBS 啟動初期 (obs_module_load)，前端 API 可能尚未準備就緒。
	// 若未將 scenes 初始化，API 沒設值會導致 num 變成垃圾記憶體而引發 SIGSEGV 崩潰。
	struct obs_frontend_source_list scenes = {0};
	obs_frontend_get_scenes(&scenes);
	for (size_t i = 0; i < scenes.sources.num; i++) {
		obs_source_t *scene = scenes.sources.array[i];
		if (!scene)
			continue;
		std::string name = obs_source_get_name(scene);
		groups.scenes.push_back(name);

		// 避免與 EnumSources 中重複加入
		auto it = std::find(groups.sources.begin(), groups.sources.end(), name);
		if (it != groups.sources.end()) {
			groups.sources.erase(it);
		}
	}
	obs_frontend_source_list_free(&scenes);

	return groups;
}

int OmniProjectorManager::GetMonitorCount()
{
	return QGuiApplication::screens().size();
}
