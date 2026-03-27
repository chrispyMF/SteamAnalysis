#include <SteamAnalysis/steam/SteamWebApiClient.h>

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
