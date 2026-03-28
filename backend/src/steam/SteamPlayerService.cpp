#include <SteamAnalysis/steam/SteamPlayerService.h>

SteamPlayerService::SteamPlayerService(SteamApiClient& apiClient) 
	:apiClient_(apiClient) {}

SteamPlayer SteamPlayerService::loadPlayer(const std::string& steamId) {
	SteamPlayer player(steamId);	
	
	PlayerSummary summary;
	auto summaryApiResult = apiClient_.getPlayerSummaries(steamId);
	if (summaryApiResult.ok() && !summaryApiResult.data["response"]["players"].empty()) {
		summary.steamId = summaryApiResult.data["response"]["players"][0]["steamid"];
		summary.personaName = summaryApiResult.data["response"]["players"][0]["personaname"];
		summary.profileUrl = summaryApiResult.data["response"]["players"][0]["profileurl"];
		summary.avatar = summaryApiResult.data["response"]["players"][0]["avatar"];
		summary.avatarMedium = summaryApiResult.data["response"]["players"][0]["avatarmedium"];
		summary.avatarFull = summaryApiResult.data["response"]["players"][0]["avatarfull"];
		summary.locCountryCode = summaryApiResult.data["response"]["players"][0]["loccountrycode"];
	}
	player.setSummary(summary);

	OwnedGames ownedGames;
	auto ownedGamesApiResult = apiClient_.getGetOwnedGames(steamId);
	if (ownedGamesApiResult.ok() && !ownedGamesApiResult.data["response"].empty()) {
		ownedGames.gameCount = ownedGamesApiResult.data["response"]["game_count"];
		for (const auto& item : ownedGamesApiResult.data["response"]["games"]) {
			Game currentGame;
			currentGame.name = item["name"];
			currentGame.imgIconUrl = item["img_icon_url"];
			currentGame.appId = item["appid"];
			currentGame.playtimeForever = item["playtime_forever"];
			ownedGames.gameList.push_back(currentGame);
		}
	}
	player.setOwnedGames(ownedGames);

	RecentlyPlayedGames recentGames;
	auto recentlyPlayedGamesApiResult = apiClient_.getRecentlyPlayedGames(steamId);
	if (recentlyPlayedGamesApiResult.ok() && !recentlyPlayedGamesApiResult.data["response"].empty()) {
		recentGames.gameCount = recentlyPlayedGamesApiResult.data["response"]["total_count"];
		for (const auto& item : recentlyPlayedGamesApiResult.data["response"]["games"]) {
			Game currentGame;
			currentGame.name = item["name"];
			currentGame.imgIconUrl = item["img_icon_url"];
			currentGame.appId = item["appid"];
			currentGame.playtimeForever = item["playtime_forever"];
			currentGame.playtimeTwoWeeks = item["playtime_2weeks"];
			recentGames.gameList.push_back(currentGame);
		}
	}
	player.setRecentlyPlayedGames(recentGames);

	return player;
}