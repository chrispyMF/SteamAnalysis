# pragma once

#include <iostream>
#include <string>
#include <atomic>
#include <curl/curl.h>
#include <SteamAnalysis/Helper.h>
#include <SteamAnalysis/logging/Logger.h>

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

