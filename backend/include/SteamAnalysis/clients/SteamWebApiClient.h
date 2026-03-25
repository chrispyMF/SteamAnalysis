#pragma once

#include <iostream>
#include <string>
#include <atomic>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <SteamAnalysis/helper.h>
#include <SteamAnalysis/logging/logger.h>

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
	std::string STEAM_API_KEY = loadEnvFile(findFile(".env").string());
	std::string STEAM_API_BASE_URL = "https://api.steampowered.com";
};

struct HttpResult {
	ApiError error = ApiError::None;
	std::string body = "";
	std::string errorMessage = "";
	long statusCode = 0;

	[[nodiscard]] bool ok() const {
		return error == ApiError::None;
	}
};

class CurlHttpClient {
public:
	explicit CurlHttpClient(Logger& logger);

	HttpResult get(const std::string& url);
private:
	Logger& logger_;
	inline static std::atomic<unsigned long long> nextRequestId_{ 1 };

	std::string makeRequestId();
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