#include <SteamAnalysis/clients/HttpClient.h>

CurlHttpClient::CurlHttpClient(Logger& logger)
	: logger_(logger) {}

std::string CurlHttpClient::makeRequestId() {
	return std::to_string(nextRequestId_++);
}

HttpResult CurlHttpClient::get(const std::string& url) {
	HttpResult result;
	const std::string requestId = makeRequestId();	
	logger_.debug("[req=" + requestId + "] Starting GET " + sanitizeUrl(url));
	
	// to get elapsed time
	auto start = std::chrono::steady_clock::now();	

	CURL* curl = curl_easy_init();
	if (!curl) {
		result.error = ApiError::CurlInitError;
		result.errorMessage = "Failed to initialize CURL.\n";
		logger_.error("[req=" + requestId + "] curl_easy_init failed.");
		curl_easy_cleanup(curl);
		return result;
	}

	std::string response;
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	CURLcode res = curl_easy_perform(curl);

	// save elapsed time in milliseconds
	auto end = std::chrono::steady_clock::now();
	auto durationMs = 
		std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

	if (res != CURLE_OK) {
		result.error = ApiError::NetworkError;
		result.errorMessage = curl_easy_strerror(res);
		logger_.error(
			"[req=" + requestId + "] GET failed"
			" url=" + sanitizeUrl(url) +
			" curl_code=" + std::to_string(static_cast<int>(res)) +
			" curl_error=\"" + result.errorMessage + "\"" +
			" duration(ms)=" + std::to_string(durationMs)
		);
		curl_easy_cleanup(curl);
		return result;
	}

	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &result.statusCode);

	if (result.statusCode >= 400) {
		result.error = ApiError::HttpError;
		result.errorMessage = "HTTP request failed with status " + std::to_string(result.statusCode);
		logger_.warn(
			"[req=" + requestId + "] GET returned HTTP error"
			" url=" + sanitizeUrl(url) +
			" status=" + std::to_string(result.statusCode) +
			" bytes=" + std::to_string(result.body.size()) +
			" duration(ms)=" + std::to_string(durationMs)
		);
		curl_easy_cleanup(curl);
		return result;
	}

	result.body = response;
	if (result.body.empty()) {
		result.error = ApiError::EmptyResponse;
		result.errorMessage = "Response body was empty.";
		logger_.warn(
			"[req=" + requestId + "] GET returned empty body"
			" url=" + sanitizeUrl(url) + 
			" status=" + std::to_string(result.statusCode) + 
			" bytes=" + std::to_string(result.body.size()) + 
			" duration(ms)=" + std::to_string(durationMs)
		);
		curl_easy_cleanup(curl);
		return result;
	}

	logger_.info(
		"[req=" + requestId + "] GET succeeded"
		" url=" + sanitizeUrl(url) + 
		" status=" + std::to_string(result.statusCode) +
		" bytes=" + std::to_string(result.body.size()) +
		" duration(ms)=" + std::to_string(durationMs)
	);
	result.error = ApiError::None;	
	curl_easy_cleanup(curl);
	return result;
}
