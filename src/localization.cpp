#include "localization.hpp"
#include <fstream>
#include <sstream>

LocalizationBundle LocalizationLoader::LoadBundle(const std::string &languageCode)
{
    LocalizationBundle bundle;
    bundle.languageCode = languageCode;

    // Try to load from data/locale/<languageCode>.ini relative to repository
    std::string path = std::string("data/locale/") + languageCode + ".ini";
    std::ifstream ifs(path);
    if (ifs) {
        std::string line;
        while (std::getline(ifs, line)) {
            // skip comments and empty lines
            if (line.empty() || line[0] == '#')
                continue;
            auto pos = line.find('=');
            if (pos == std::string::npos)
                continue;
            std::string key = line.substr(0, pos);
            std::string val = line.substr(pos + 1);
            // trim
            auto trim = [](std::string &s) {
                while (!s.empty() && (s.back() == '\r' || s.back() == '\n'))
                    s.pop_back();
                size_t start = 0;
                while (start < s.size() && isspace((unsigned char)s[start]))
                    start++;
                if (start)
                    s = s.substr(start);
                size_t end = s.size();
                while (end > 0 && isspace((unsigned char)s[end - 1]))
                    end--;
                s = s.substr(0, end);
            };
            trim(key);
            trim(val);
            if (!key.empty())
                bundle.translations[key] = val;
        }
    }

    return bundle;
}

std::vector<std::string> LocalizationLoader::ExportMissingKeys(const LocalizationBundle &bundle, const std::string &exportPath, const std::string &format)
{
    std::vector<std::string> missing;
    // Skeleton: write empty report file to satisfy feature flow
    if (format == "csv") {
        std::ofstream ofs(exportPath);
        ofs << "key\n";
        for (const auto &k : missing)
            ofs << k << "\n";
        ofs.close();
    } else {
        std::ofstream ofs(exportPath);
        ofs << "{}";
        ofs.close();
    }

    return missing;
}

// LocalizationManager static state
LocalizationBundle LocalizationManager::bundle_ = LocalizationBundle();

void LocalizationManager::Initialize(const std::string &languageCode)
{
    bundle_ = LocalizationLoader::LoadBundle(languageCode);
}

std::string LocalizationManager::Translate(const std::string &key)
{
    auto it = bundle_.translations.find(key);
    if (it != bundle_.translations.end())
        return it->second;
    return std::string();
}

std::string LocalizationManager::CurrentLanguage()
{
    return bundle_.languageCode;
}
