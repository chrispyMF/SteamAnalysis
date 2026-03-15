#pragma once

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <SteamAnalysis/clients/helper.h>

using json = nlohmann::json;

struct SteamClientConfig {
	std::string STEAM_API_KEY = loadEnvFile(findEnvFile().string());
	std::string STEAM_API_BASE_URL = "https://api.steampowered.com";
};

class CurlHttpClient {
public:
	std::string get(const std::string& url);
};

class SteamApiClient {
public:
	json getPlayerSummaries(const std::string& steamId);
private:
	SteamClientConfig config_;
	std::shared_ptr<CurlHttpClient> http_;

	std::string buildUrl(const std::string& path, const std::map < std::string, std::string>& queryParams) const;
};