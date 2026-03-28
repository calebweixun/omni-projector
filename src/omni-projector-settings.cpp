#include "omni-projector-settings.hpp"
#include <obs-module.h>
#include <obs-frontend-api.h>
#include <util/config-file.h>
#include <util/platform.h>

// 取得當前 OBS 設定檔目錄下的插件配置路徑
static std::string GetProfileSettingsPath()
{
	char *profile_path = obs_frontend_get_current_profile_path();
	if (!profile_path) {
		// fallback：若前端 API 尚未就緒，使用插件自身目錄
		char *fallback = obs_module_get_config_path(obs_current_module(), "settings.json");
		std::string result(fallback);
		bfree(fallback);
		return result;
	}

	std::string path(profile_path);
	path += "/omni-projector.json";
	bfree(profile_path);
	return path;
}

void OmniProjectorSettings::Save(const std::vector<MappingEntry> &mappings)
{
	// Load existing data if present to avoid clobbering other keys
	std::string path = GetProfileSettingsPath();
	obs_data_t *data = obs_data_create_from_json_file(path.c_str());
	if (!data)
		data = obs_data_create();

	obs_data_array_t *array = obs_data_array_create();
	for (const auto &entry : mappings) {
		obs_data_t *obj = obs_data_create();
		obs_data_set_string(obj, "source", entry.source_name.c_str());
		obs_data_set_int(obj, "monitor", entry.monitor_index);
		obs_data_array_push_back(array, obj);
		obs_data_release(obj);
	}

	obs_data_set_array(data, "mappings", array);
	obs_data_save_json(data, path.c_str());

	obs_data_array_release(array);
	obs_data_release(data);
}

std::vector<MappingEntry> OmniProjectorSettings::Load()
{
	std::vector<MappingEntry> mappings;
	std::string path = GetProfileSettingsPath();

	obs_data_t *data = obs_data_create_from_json_file(path.c_str());
	if (data) {
		obs_data_array_t *array = obs_data_get_array(data, "mappings");
		if (array) {
			size_t count = obs_data_array_count(array);
			for (size_t i = 0; i < count; i++) {
				obs_data_t *obj = obs_data_array_item(array, i);
				mappings.push_back(
					{obs_data_get_string(obj, "source"), (int)obs_data_get_int(obj, "monitor")});
				obs_data_release(obj);
			}
			obs_data_array_release(array);
		}

		obs_data_release(data);
	}

	return mappings;
}

void OmniProjectorSettings::SaveLanguageSettings(const std::string &languageMode, const std::string &selectedLanguage)
{
	std::string path = GetProfileSettingsPath();
	obs_data_t *data = obs_data_create_from_json_file(path.c_str());
	if (!data)
		data = obs_data_create();

	obs_data_set_string(data, "language_mode", languageMode.c_str());
	obs_data_set_string(data, "selected_language", selectedLanguage.c_str());

	obs_data_save_json(data, path.c_str());
	obs_data_release(data);
}

void OmniProjectorSettings::LoadLanguageSettings(std::string &languageMode, std::string &selectedLanguage)
{
	languageMode.clear();
	selectedLanguage.clear();
	std::string path = GetProfileSettingsPath();
	obs_data_t *data = obs_data_create_from_json_file(path.c_str());
	if (!data)
		return;

	const char *lm = obs_data_get_string(data, "language_mode");
	const char *sl = obs_data_get_string(data, "selected_language");
	if (lm)
		languageMode = lm;
	if (sl)
		selectedLanguage = sl;

	obs_data_release(data);
}
