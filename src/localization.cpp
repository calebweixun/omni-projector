#include "localization.hpp"
#include <fstream>
#include <sstream>

LocalizationBundle LocalizationLoader::LoadBundle(const std::string &languageCode)
{
    LocalizationBundle bundle;
    bundle.languageCode = languageCode;

    // Skeleton: load from plugin resource path (to be implemented)
    // For now return empty translations to allow wiring

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
