#include "omni-projector-manager.hpp"
#include <obs-frontend-api.h>

OmniProjectorManager& OmniProjectorManager::Get()
{
    static OmniProjectorManager instance;
    return instance;
}

void OmniProjectorManager::StartProjection(obs_source_t *source, int monitor_id)
{
    if (!source) return;
    
    const char *name = obs_source_get_name(source);
    // 這裡暫時使用 OBS 原生投影
    obs_frontend_open_source_projector("default", name, monitor_id, nullptr);
}

void OmniProjectorManager::StopAllProjections()
{
    // 目前沒有直接關閉所有投影的 API，通常需要手動追蹤或使用 OBS 指令
}

std::vector<std::string> OmniProjectorManager::GetAvailableSources()
{
    std::vector<std::string> source_list;
    
    auto EnumSources = [](void *data, obs_source_t *source) {
        auto *list = static_cast<std::vector<std::string>*>(data);
        uint32_t caps = obs_source_get_output_flags(source);
        
        // 只列出可投影的來源 (場景或實體來源)
        if (caps & OBS_SOURCE_VIDEO) {
            list->push_back(obs_source_get_name(source));
        }
        return true;
    };
    
    obs_enum_sources(EnumSources, &source_list);
    
    return source_list;
}

int OmniProjectorManager::GetMonitorCount()
{
    // 簡單回傳螢幕數量 (實際上應從 Qt 取得更準確)
    return 2; 
}
