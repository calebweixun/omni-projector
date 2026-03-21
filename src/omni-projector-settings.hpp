#pragma once

#include <obs.h>
#include <string>
#include <vector>

struct MappingEntry {
    std::string source_name;
    int monitor_index;
};

class OmniProjectorSettings {
public:
    static void Save(const std::vector<MappingEntry>& mappings);
    static std::vector<MappingEntry> Load();

private:
    static const char* SETTINGS_PATH;
};
