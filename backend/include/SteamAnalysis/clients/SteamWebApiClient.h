#pragma once

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <SteamAnalysis/utils/Helper.h>
#include <SteamAnalysis/clients/HttpClient.h>
#include <SteamAnalysis/logging/Logger.h>

using json = nlohmann::json;

/// Struct that stores API key and base url
/// for Steam API.
struct SteamClientConfig {
	std::string STEAM_API_KEY = loadEnvFile(findFile(".env").string());
	std::string STEAM_API_BASE_URL = "https://api.steampowered.com";
};

/// Struct that stores result from a Steam API request. 
struct SteamApiResult {
	ApiError error = ApiError::None;
	json data = nullptr;
	std::string errorMessage = "";
	long statusCode = 0;

	/// Function to check if the request returned an error.
	/// @return True/False
	[[nodiscard]] bool ok() const {
		return error == ApiError::None;
	}
};

/// Http client for Steam API. Makes Http request with a CurlHttpClient object.
/// Logs errors and info with Logger object.
///	Contains methods that call GetPlayerSummaries, GetOwnedGames, and GetRecentlyPlayedGames from Steam API.
/// @param logger Logger object that contains log file.
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

	/// Makes the API request url using a path and query parameters.
	/// @param path API request url path as a string.
	/// @param queryParams std::map that contains query parameters.
	/// @return String url.
	std::string buildUrl(const std::string& path, const std::map<std::string, std::string>& queryParams) const;
};