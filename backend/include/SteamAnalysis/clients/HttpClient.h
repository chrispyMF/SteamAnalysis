# pragma once

#include <iostream>
#include <string>
#include <atomic>
#include <curl/curl.h>
#include <SteamAnalysis/utils/Helper.h>
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
	Unknown
};

/// Struct to save result from Http requests.
/// This struct is used by the Http client to store information from requests.
struct HttpResult {
	/// Type of error returned, whether caused by CURL, Steam API, or during parsing. 
	ApiError error = ApiError::None;

	/// Http request response body. 
	std::string body = "";

	/// Http request error. 
	std::string errorMessage = "";

	/// Http request status code. 
	long statusCode = 0;

	/// Function to check if the request returned an error.
	/// @return True/False
	[[nodiscard]] bool ok() const {
		return error == ApiError::None;
	}
};

/// Http client class that sends Http requests.
/// Stores results from requests in a HttpResult struct.
/// @param logger Logger object for logging. 
/// @return HttpResult struct.
class CurlHttpClient {
public:
	explicit CurlHttpClient(Logger& logger);

	/// Function that performs Http GET request.
	/// @param url API request url string.
	/// @return HttpResult struct.
	HttpResult get(const std::string& url);

private:
	Logger& logger_;
	inline static std::atomic<unsigned long long> nextRequestId_{ 1 };

	std::string makeRequestId();
};

