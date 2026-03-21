/*
 * OmniProjector - OBS Matrix Switcher & Monitor
 * Copyright (C) 2026 Caleb

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include <obs-module.h>
#include <obs-frontend-api.h>
#include "omni-projector-dock.hpp"
#include "omni-projector-manager.hpp"
#include <plugin-support.h>

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

// 前端事件回呼：處理設定檔切換與程式退出
static void OnFrontendEvent(enum obs_frontend_event event, void *data)
{
	(void)data;
	switch (event) {
	case OBS_FRONTEND_EVENT_PROFILE_CHANGED:
		// 設定檔切換時，重新載入該設定檔的投影配置
		obs_log(LOG_INFO, "OmniProjector: 設定檔已切換，重新載入配置");
		OmniProjectorManager::Get().LoadSettings();
		OmniProjectorDock::RefreshAll();
		break;
	case OBS_FRONTEND_EVENT_EXIT:
		// OBS 關閉前自動保存配置
		obs_log(LOG_INFO, "OmniProjector: OBS 結束中，保存配置");
		OmniProjectorManager::Get().SaveSettings();
		break;
	default:
		break;
	}
}

extern "C" bool obs_module_load(void)
{
	obs_log(LOG_INFO, "OmniProjector loaded (version %s)", PLUGIN_VERSION);

	OmniProjectorDock::Register();

	// 註冊前端事件回呼
	obs_frontend_add_event_callback(OnFrontendEvent, nullptr);

	return true;
}

extern "C" void obs_module_unload(void)
{
	obs_frontend_remove_event_callback(OnFrontendEvent, nullptr);
	obs_log(LOG_INFO, "OmniProjector unloaded");
}
