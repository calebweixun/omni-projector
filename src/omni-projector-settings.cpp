#include "omni-projector-settings.hpp"
#include <obs-module.h>
#include <util/config-file.h>

void OmniProjectorSettings::Save(const std::vector<MappingEntry> &mappings)
{
	obs_data_t *data = obs_data_create();
	obs_data_array_t *array = obs_data_array_create();

	for (const auto &entry : mappings) {
		obs_data_t *obj = obs_data_create();
		obs_data_set_string(obj, "source", entry.source_name.c_str());
		obs_data_set_int(obj, "monitor", entry.monitor_index);
		obs_data_array_push_back(array, obj);
		obs_data_release(obj);
	}

	obs_data_set_array(data, "mappings", array);

	// 儲存至 OBS 的插件設定目錄
	char *bin_path = obs_module_get_config_path(obs_current_module(), "settings.json");
	obs_data_save_json(data, bin_path);

	bfree(bin_path);
	obs_data_array_release(array);
	obs_data_release(data);
}

std::vector<MappingEntry> OmniProjectorSettings::Load()
{
	std::vector<MappingEntry> mappings;
	char *bin_path = obs_module_get_config_path(obs_current_module(), "settings.json");

	obs_data_t *data = obs_data_create_from_json_file(bin_path);
	if (data) {
		obs_data_array_t *array = obs_data_get_array(data, "mappings");
		size_t count = obs_data_array_count(array);

		for (size_t i = 0; i < count; i++) {
			obs_data_t *obj = obs_data_array_item(array, i);
			mappings.push_back({obs_data_get_string(obj, "source"), (int)obs_data_get_int(obj, "monitor")});
			obs_data_release(obj);
		}

		obs_data_array_release(array);
		obs_data_release(data);
	}

	bfree(bin_path);
	return mappings;
}
