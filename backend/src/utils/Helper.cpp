#include <SteamAnalysis/utils/Helper.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

std::string buildQuery(const std::string& key, const std::string& value) {
    std::string query = "&";
    return query.append(key + "=" + value);
}

std::string sanitizeUrl(const std::string& url) {
    return std::regex_replace(url, std::regex(R"((key=)[^&]+)"), "$1****");
}

std::filesystem::path findFile(const std::string& fileName) {
    auto dir = std::filesystem::current_path();
    while (!dir.empty()) {
        auto candidate = dir / fileName;
        if (std::filesystem::exists(candidate)) {
            return candidate;
        }
        dir = dir.parent_path();
    }
    throw std::runtime_error("File not found");
}

std::string loadEnvFile(const std::string& file) {
    std::ifstream f(file);
    std::unordered_map<std::string, std::string> env;
    std::string line;

    while (std::getline(f, line)) {
        auto pos = line.find('=');
        if (pos == std::string::npos) {
            continue;
        }
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        env[key] = value;
    }
    return env["STEAM_API_KEY"];
}
