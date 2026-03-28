#pragma once

#include <string>
#include <map>
#include <vector>

struct LocalizationBundle {
    std::string languageCode;
    std::map<std::string, std::string> translations;
};

class LocalizationLoader {
public:
    static LocalizationBundle LoadBundle(const std::string &languageCode);
    static std::vector<std::string> ExportMissingKeys(const LocalizationBundle &bundle, const std::string &exportPath, const std::string &format /* csv|json */);
};
