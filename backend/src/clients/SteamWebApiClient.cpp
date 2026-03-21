#include <SteamAnalysis/clients/SteamWebApiClient.h>

HttpResult CurlHttpClient::get(const std::string& url) {
	HttpResult result;
	CURL* curl = curl_easy_init();
	if (!curl) {
		result.error = ApiError::CurlInitError;
		result.errorMessage = "Failed to initialize CURL.\n";
		return result;
	}

	std::string response;
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		result.error = ApiError::NetworkError;
		result.errorMessage = curl_easy_strerror(res);
		return result;
	}

	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &result.statusCode);
	if (result.statusCode >= 400) {
		result.error = ApiError::HttpError;
		result.errorMessage = "HTTP request failed with status " + std::to_string(result.statusCode);
		return result;
	}

	result.body = response;
	if (result.body.empty()) {
		result.error = ApiError::EmptyResponse;
		result.errorMessage = "Response body was empty.";
		return result;
	}

	result.error = ApiError::None;	
	curl_easy_cleanup(curl);
	return result;
}

SteamApiResult SteamApiClient::getPlayerSummaries(const std::string& steamId) {
	SteamApiResult result;
	
	std::string url = buildUrl("/ISteamUser/GetPlayerSummaries/v0002/",
		{ {"key", config_.STEAM_API_KEY}, {"steamids", steamId}, {"format", "json"} }
	);
	HttpResult httpResult = http_->get(url);
	result.statusCode = httpResult.statusCode;

	if (!httpResult.ok()) {
		result.error = httpResult.error;
		result.errorMessage = httpResult.errorMessage;
		return result;
	}

	json resultJson = json::parse(httpResult.body);
	if (resultJson.is_discarded()) {
		result.error = ApiError::JsonParseError;
		result.errorMessage = "Failed to parse JSON from Steam response.";
		return result;
	}

	if (!resultJson.contains("response") || !resultJson["response"].is_object()) {
		result.error = ApiError::SteamInvalidResponse;
		result.errorMessage = "Steam response missing 'response' object.";
		return result;
	}

	if (!resultJson["response"].contains("players") || !resultJson["response"]["players"].is_array()) {
		result.error = ApiError::SteamInvalidResponse;
		result.errorMessage = "Steam response missing 'players' array.";
		return result;
	}

	if (resultJson["response"]["players"].empty()) {
		result.error = ApiError::SteamInvalidResponse;
		result.errorMessage = "No players found for provided Steam ID.";
		return result;
	}

	result.error = ApiError::None;
	result.data = resultJson;
	return result;
}

SteamApiResult SteamApiClient::getGetOwnedGames(const std::string& steamId) {
	SteamApiResult result;

	std::string url = buildUrl("/IPlayerService/GetOwnedGames/v0001/" , 
		{ {"key", config_.STEAM_API_KEY}, {"steamid", steamId}, {"include_appinfo", "true"}, 
			{"include_played_free_games", "true"}, {"format", "json"}}
	);
	HttpResult httpResult = http_->get(url);
	result.statusCode = httpResult.statusCode;

	if (!httpResult.ok()) {
		result.error = httpResult.error;
		result.errorMessage = httpResult.errorMessage;
		return result;
	}

	json resultJson = json::parse(httpResult.body);
	if (resultJson.is_discarded()) {
		result.error = ApiError::JsonParseError;
		result.errorMessage = "Failed to parse JSON from Steam response.";
	}

	if (!resultJson.contains("response") || !resultJson["response"].is_object()) {
		result.error = ApiError::SteamInvalidResponse;
		result.errorMessage = "Steam response missing 'response' object.";
		return result;
	}
	
	if (!resultJson["response"].contains("game_count") || !resultJson["response"]["game_count"].is_number_integer()) {
		result.error = ApiError::SteamInvalidResponse;
		result.errorMessage = "Steam response missing 'game_count' number.";
		return result;
	}

	if (!resultJson["response"].contains("games") || !resultJson["response"]["games"].is_array()) {
		result.error = ApiError::SteamInvalidResponse;
		result.errorMessage = "Steam response missing 'games' array.";
		return result;
	}

	if (resultJson["response"]["games"].empty()) {
		result.error = ApiError::SteamEmptyResponse;
		result.errorMessage = "No games found for provided Steam ID.";
		return result;
	}
	
	result.error = ApiError::None;
	result.data = resultJson;
	return result;
}

SteamApiResult SteamApiClient::getRecentlyPlayedGames(const std::string& steamId) {
	SteamApiResult result;

	std::string url = buildUrl("/IPlayerService/GetRecentlyPlayedGames/v0001/",
		{{"key", config_.STEAM_API_KEY}, {"steamid", steamId}, {"format", "json"} }
	);
	HttpResult httpResult = http_->get(url);
	result.statusCode = httpResult.statusCode;
	
	if (!httpResult.ok()) {
		result.error = httpResult.error;
		result.errorMessage = httpResult.errorMessage;
		return result;
	}

	json resultJson = json::parse(httpResult.body);
	if (resultJson.is_discarded()) {
		result.error = ApiError::JsonParseError;
		result.errorMessage = "Failed to parse JSON from Steam response.";
		return result;
	}

	if (!resultJson.contains("response") || !resultJson["response"].is_object()) {
		result.error = ApiError::SteamInvalidResponse;
		result.errorMessage = "Steam response missing 'response' object.";
		return result;
	}

	if (!resultJson["response"].contains("total_count") || !resultJson["response"]["total_count"].is_number_integer()) {
		result.error = ApiError::SteamInvalidResponse;
		result.errorMessage = "Steam response missing 'total_count' number.";
		return result;
	}

	if (!resultJson["response"].contains("games") || !resultJson["response"]["games"].is_array()) {
		result.error = ApiError::SteamInvalidResponse;
		result.errorMessage = "Steam response missing 'games' array.";
		return result;
	}

	if (resultJson["response"]["games"].empty()) {
		result.error = ApiError::SteamEmptyResponse;
		result.errorMessage = "No games found for provided Steam ID.";
		return result;
	}

	result.error = ApiError::None;
	result.data = resultJson;
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
