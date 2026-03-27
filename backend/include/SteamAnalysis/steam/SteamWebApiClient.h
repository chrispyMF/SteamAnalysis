#pragma once

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <SteamAnalysis/Helper.h>
#include <SteamAnalysis/http/HttpClient.h>
#include <SteamAnalysis/logging/Logger.h>

using json = nlohmann::json;

struct SteamClientConfig {
	std::string STEAM_API_KEY = loadEnvFile(findFile(".env").string());
	std::string STEAM_API_BASE_URL = "https://api.steampowered.com";
};

struct SteamApiResult {
	ApiError error = ApiError::None;
	json data = nullptr;
	std::string errorMessage = "";
	long statusCode = 0;

	[[nodiscard]] bool ok() const {
		return error == ApiError::None;
	}
};

class SteamApiClient {
public:
	explicit SteamApiClient(Logger& logger);

	SteamApiResult getPlayerSummaries(const std::string& steamId);
	SteamApiResult getGetOwnedGames(const std::string& steamId);
	SteamApiResult getRecentlyPlayedGames(const std::string& steamId);

private:
	SteamClientConfig config_;
	CurlHttpClient http_;
	Logger& logger_;

	std::string buildUrl(const std::string& path, const std::map<std::string, std::string>& queryParams) const;
};