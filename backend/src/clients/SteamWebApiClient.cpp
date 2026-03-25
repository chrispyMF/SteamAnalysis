#include <SteamAnalysis/clients/SteamWebApiClient.h>

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

SteamApiClient::SteamApiClient(Logger& logger)
	: logger_(logger), http_(logger){}

SteamApiResult SteamApiClient::getPlayerSummaries(const std::string& steamId) {
	SteamApiResult result;
	
	logger_.debug("SteamApiClient::getPlayerSummaries steamId=" + steamId); 
	
	std::string url = buildUrl("/ISteamUser/GetPlayerSummaries/v0002/",
		{ {"key", config_.STEAM_API_KEY}, {"steamids", steamId}, {"format", "json"} }
	);
	HttpResult httpResult = http_.get(url);
	result.statusCode = httpResult.statusCode;

	if (!httpResult.ok()) {
		result.error = httpResult.error;
		result.errorMessage = httpResult.errorMessage;
		logger_.error(
			"Steam getPlayerSummaries failed"
			" steamId=" + steamId +
			" api_error=" + std::to_string(static_cast<int>(result.error)) +
			" message=\"" + result.errorMessage + "\""
			" status=" + std::to_string(result.statusCode)
		);
		return result;
	}

	json resultJson = json::parse(httpResult.body);
	if (resultJson.is_discarded()) {
		result.error = ApiError::JsonParseError;
		result.errorMessage = "Failed to parse JSON from Steam response.";	
		logger_.error(
			"Steam getPlayerSummaries parse error"
			" steamId=" + steamId + 
			" message=\"" + result.errorMessage + "\""
			" status=" + std::to_string(result.statusCode)
		);
		return result;
	}

	if (!resultJson.contains("response") || !resultJson["response"].is_object()) {
		result.error = ApiError::SteamInvalidResponse;
		result.errorMessage = "Steam response missing 'response' object.";	
		logger_.warn(
			"Steam getPlayerSummaries invalid JSON structure"
			" steamId=" + steamId +
			" message=\"" + result.errorMessage + "\"" 
			" status=" + std::to_string(result.statusCode)
		);
		return result;
	}

	if (!resultJson["response"].contains("players") || !resultJson["response"]["players"].is_array()) {
		result.error = ApiError::SteamInvalidResponse;
		result.errorMessage = "Steam response missing 'players' array.";
		logger_.warn(
			"Steam getPlayerSummaries invalid JSON structure"
			" steamId=" + steamId +
			" message=\"" + result.errorMessage + "\""
			" status=" + std::to_string(result.statusCode)
		);
		return result;
	}

	if (resultJson["response"]["players"].empty()) {
		result.error = ApiError::SteamInvalidResponse;
		result.errorMessage = "No players found for provided Steam ID.";
		logger_.warn(
			"Steam getPlayerSummaries Steam ID error"
			" steamId=" + steamId +
			" message=\"" + result.errorMessage + "\""
			" status=" + std::to_string(result.statusCode)
		);
		return result;
	}

	result.error = ApiError::None;
	result.data = resultJson;
	logger_.info("Steam getPlayerSummaires succeeded steamId=" + steamId);
	return result;
}

SteamApiResult SteamApiClient::getGetOwnedGames(const std::string& steamId) {
	SteamApiResult result;

	logger_.debug("SteamApiClient::getGetOwnedGames steamId=" + steamId);

	std::string url = buildUrl("/IPlayerService/GetOwnedGames/v0001/" , 
		{ {"key", config_.STEAM_API_KEY}, {"steamid", steamId}, {"include_appinfo", "true"}, 
			{"include_played_free_games", "true"}, {"format", "json"}}
	);
	HttpResult httpResult = http_.get(url);
	result.statusCode = httpResult.statusCode;

	if (!httpResult.ok()) {
		result.error = httpResult.error;
		result.errorMessage = httpResult.errorMessage;
		logger_.error(
			"Steam getGetOwnedGames failed"
			" steamId=" + steamId +
			" api_error=" + std::to_string(static_cast<int>(result.error)) +
			" message=\"" + result.errorMessage = "\""
			" status=" + std::to_string(result.statusCode)
		);	
		return result;
	}

	json resultJson = json::parse(httpResult.body);
	if (resultJson.is_discarded()) {
		result.error = ApiError::JsonParseError;
		result.errorMessage = "Failed to parse JSON from Steam response.";	
		logger_.error(
			"Steam getGetOwnedGames parse error"
			" steamId=" + steamId +
			" message=\"" + result.errorMessage + "\""
			" status=" + std::to_string(result.statusCode)
		);
		return result;
	}

	if (!resultJson.contains("response") || !resultJson["response"].is_object()) {
		result.error = ApiError::SteamInvalidResponse;
		result.errorMessage = "Steam response missing 'response' object.";
		logger_.warn(
			"Steam getGetOwnedGames invalid JSON structure"
			" steamId=" + steamId +
			" message=\"" + result.errorMessage + "\""
			" status=" + std::to_string(result.statusCode)
		);
		return result;
	}
	
	if (!resultJson["response"].contains("game_count") || !resultJson["response"]["game_count"].is_number_integer()) {
		result.error = ApiError::SteamInvalidResponse;
		result.errorMessage = "Steam response missing 'game_count' number.";
		logger_.warn(
			"Steam getGetOwnedGames invalid JSON structure"
			" steamId=" + steamId +
			" message=\"" + result.errorMessage + "\""
			" status=" + std::to_string(result.statusCode)
		);
		return result;
	}

	if (!resultJson["response"].contains("games") || !resultJson["response"]["games"].is_array()) {
		result.error = ApiError::SteamInvalidResponse;
		result.errorMessage = "Steam response missing 'games' array.";
		logger_.warn(
			"Steam getGetOwnedGames invalid JSON structure"
			" steamId=" + steamId +
			" message=\"" + result.errorMessage + "\""
			" status=" + std::to_string(result.statusCode)
		);
		return result;
	}

	if (resultJson["response"]["games"].empty()) {
		result.error = ApiError::SteamEmptyResponse;
		result.errorMessage = "No games found for provided Steam ID.";
		logger_.warn(
			"Steam getGetOwnedGames found not games"
			" steamId=" + steamId +
			" message=\"" + result.errorMessage + "\""
			" status=" + std::to_string(result.statusCode)
		);
		return result;
	}
	
	result.error = ApiError::None;
	result.data = resultJson;
	logger_.info("Steam getGetOwnedGames succeeded steamId=" + steamId);
	return result;
}

SteamApiResult SteamApiClient::getRecentlyPlayedGames(const std::string& steamId) {
	SteamApiResult result;

	logger_.debug("SteamApiClient::getRecentlyPlayedGames steamId=" + steamId);

	std::string url = buildUrl("/IPlayerService/GetRecentlyPlayedGames/v0001/",
		{{"key", config_.STEAM_API_KEY}, {"steamid", steamId}, {"format", "json"} }
	);
	HttpResult httpResult = http_.get(url);
	result.statusCode = httpResult.statusCode;
	
	if (!httpResult.ok()) {
		result.error = httpResult.error;
		result.errorMessage = httpResult.errorMessage;
		logger_.error(
			"Steam getRecentlyPlayedGames failed"
			" steamId=" + steamId +
			" api_error=" + std::to_string(static_cast<int>(result.error)) +
			" message=\"" + result.errorMessage + "\""
			" status=" + std::to_string(result.statusCode)
		);
		return result;
	}

	json resultJson = json::parse(httpResult.body);
	if (resultJson.is_discarded()) {
		result.error = ApiError::JsonParseError;
		result.errorMessage = "Failed to parse JSON from Steam response.";
		logger_.error(
			"Steam getRecentlyPlayedGames parse error"
			" steamId=" + steamId +
			" message=\"" + result.errorMessage + "\""
			" status=" + std::to_string(result.statusCode)
		);
		return result;
	}

	if (!resultJson.contains("response") || !resultJson["response"].is_object()) {
		result.error = ApiError::SteamInvalidResponse;
		result.errorMessage = "Steam response missing 'response' object.";
		logger_.warn(
			"Steam getRecentlyPlayedGames invalid JSON structure"
			" steamId=" + steamId +
			" message=\"" + result.errorMessage + "\""
			" status=" + std::to_string(result.statusCode)
		);
		return result;
	}

	if (!resultJson["response"].contains("total_count") || !resultJson["response"]["total_count"].is_number_integer()) {
		result.error = ApiError::SteamInvalidResponse;
		result.errorMessage = "Steam response missing 'total_count' number.";
		logger_.warn(
			"Steam getRecentlyPlayedGames invalid JSON structure"
			" steamId=" + steamId +
			" message=\"" + result.errorMessage + "\""
			" status=" + std::to_string(result.statusCode)
		);
		return result;
	}

	if (!resultJson["response"].contains("games") || !resultJson["response"]["games"].is_array()) {
		result.error = ApiError::SteamInvalidResponse;
		result.errorMessage = "Steam response missing 'games' array.";
		logger_.warn(
			"Steam getRecentlyPlayedGames invalid JSON structure"
			" steamId=" + steamId +
			" message=\"" + result.errorMessage + "\""
			" status=" + std::to_string(result.statusCode)
		);
		return result;
	}

	if (resultJson["response"]["games"].empty()) {
		result.error = ApiError::SteamEmptyResponse;
		result.errorMessage = "No games found for provided Steam ID.";
		logger_.warn(
			"Steam getRecentlyPlayedGames found no games"
			" steamId=" + steamId +
			" message=\"" + result.errorMessage + "\""
			" status=" + std::to_string(result.statusCode)
		);
		return result;
	}

	result.error = ApiError::None;
	result.data = resultJson;
	logger_.info("Steam getRecentlyPlayedGames succeeded steamId=" + steamId);
	return result;
}

std::string SteamApiClient::buildUrl(const std::string& path, const std::map<std::string, std::string>& queryParams) const {
	std::string url;
	url.append(config_.STEAM_API_BASE_URL + path + "?");
	for (const auto& pair : queryParams) {
		url.append(buildQuery(pair.first, pair.second));
	}
	return url;
}
