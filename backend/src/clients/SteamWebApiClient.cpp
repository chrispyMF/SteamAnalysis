#include <SteamAnalysis/clients/SteamWebApiClient.h>

std::string CurlHttpClient::get(const std::string& url) {
	CURL* curl = curl_easy_init();
	if (!curl) {
		std::cerr << "Failed to initialize CURL\n";
		return "";
	}

	std::string response;
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		std::cerr << "CURL Error: " << curl_easy_strerror(res) << std::endl;
		return "";
	}
	
	curl_easy_cleanup(curl);

	return response;
}

json SteamApiClient::getPlayerSummaries(const std::string& steamId) {
	std::string url = buildUrl("/ISteamUser/GetPlayerSummaries/v0002/",
		{ {"key", config_.STEAM_API_KEY}, {"steamids", steamId}, {"format", "json"} }
	);
	std::string strResponse = http_->get(url);
	json jsonResult = json::parse(strResponse);
	return jsonResult;
}

json SteamApiClient::getGetOwnedGames(const std::string& steamId) {
	std::string url = buildUrl("/IPlayerService/GetOwnedGames/v0001/" , 
		{ {"key", config_.STEAM_API_KEY}, {"steamid", steamId}, {"include_appinfo", "true"}, 
			{"include_played_free_games", "true"}, {"format", "json"}}
	);
	std::string strResponse = http_->get(url);
	json jsonResult = json::parse(strResponse);
	return jsonResult;
}

json SteamApiClient::getRecentlyPlayedGames(const std::string& steamId) {
	std::string url = buildUrl("/IPlayerService/GetRecentlyPlayedGames/v0001/",
		{{"key", config_.STEAM_API_KEY}, {"steamid", steamId}, {"format", "json"} }
	);
	std::string strResponse = http_->get(url);
	json jsonResult = json::parse(strResponse);
	return jsonResult;
}

std::string SteamApiClient::buildUrl(const std::string& path, const std::map<std::string, std::string>& queryParams) const {
	std::string url;
	url.append(config_.STEAM_API_BASE_URL + path + "?");
	for (const auto& pair : queryParams) {
		url.append("&" + pair.first + "=" + pair.second);
	}
	return url;
}
