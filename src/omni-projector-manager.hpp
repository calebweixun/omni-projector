#pragma once

#include <obs.h>
#include <vector>
#include <string>

struct ProjectionTarget {
    std::string name;
    int monitor_id;
};

class OmniProjectorManager {
public:
    static OmniProjectorManager& Get();

    // 核心功能
    void StartProjection(obs_source_t *source, int monitor_id);
    void StopAllProjections();
    
    // 資料取得
    std::vector<std::string> GetAvailableSources();
    int GetMonitorCount();

private:
    OmniProjectorManager() = default;
    ~OmniProjectorManager() = default;
    
    // 禁止複製
    OmniProjectorManager(const OmniProjectorManager&) = delete;
    OmniProjectorManager& operator=(const OmniProjectorManager&) = delete;
};
