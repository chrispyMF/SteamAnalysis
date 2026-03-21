#pragma once

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <SteamAnalysis/clients/helper.h>

using json = nlohmann::json;

enum class ApiError {
	None,
	// libcurl errors
	CurlInitError,
	NetworkError,
	Timeout,
	HttpError,
	// parsing errors
	EmptyResponse,
	JsonParseError,
	// steam api errors
	SteamEmptyResponse,
	SteamInvalidResponse,
	// fallback	
	Unkown
};

struct SteamClientConfig {
	std::string STEAM_API_KEY = loadEnvFile(findEnvFile().string());
	std::string STEAM_API_BASE_URL = "https://api.steampowered.com";
};

struct HttpResult {
	ApiError error = ApiError::None;
	std::string body;
	std::string errorMessage;
	long statusCode = 0;

	[[nodiscard]] bool ok() const {
		return error == ApiError::None;
	}
};

class CurlHttpClient {
public:
	HttpResult get(const std::string& url);
};

struct SteamApiResult {
	ApiError error = ApiError::None;
	json data = nullptr;
	std::string errorMessage;
	long statusCode = 0;

	[[nodiscard]] bool ok() const {
		return error == ApiError::None;
	}
};

class SteamApiClient {
public:
	SteamApiResult getPlayerSummaries(const std::string& steamId);
	SteamApiResult getGetOwnedGames(const std::string& steamId);
	SteamApiResult getRecentlyPlayedGames(const std::string& steamId);
private:
	SteamClientConfig config_;
	std::shared_ptr<CurlHttpClient> http_;

	std::string buildUrl(const std::string& path, const std::map<std::string, std::string>& queryParams) const;
};