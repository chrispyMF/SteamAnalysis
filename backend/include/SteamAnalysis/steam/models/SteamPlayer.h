# pragma once

#include <string>
#include <optional>
#include <vector>
#include <utility>
#include <SteamAnalysis/steam/SteamWebApiClient.h>

struct PlayerSummary {
	std::string steamId = "";
	std::string personaName = "";
	std::string profileUrl = "";
	std::string avatar = "";
	std::string avatarMedium = "";
	std::string avatarFull = "";
	std::string locCountryCode = "";
};

struct Game {
	std::string name = "";
	std::string imgIconUrl = "";
	int appId = 0;
	int playtimeForever = 0;
	int playtimeTwoWeeks = 0;
};

struct OwnedGames {
	int gameCount = 0;
	std::vector<Game> gameList = {};
};

struct RecentlyPlayedGames {
	int gameCount = 0;
	std::vector<Game> gameList = {};
};

class SteamPlayer {
public:
	explicit SteamPlayer(std::string steamId);
	
	const std::string& steamId() const noexcept;

	// presence checks
	bool hasSummary() const noexcept;
	bool hasOwnedGames() const noexcept;
	bool hasRecentlyPlayedGames() const noexcept;

	// getters
	const std::optional<PlayerSummary>& summary() const noexcept;
	const std::optional<OwnedGames>& ownedGames() const noexcept;
	const std::optional<RecentlyPlayedGames>& recentlyPlayedGames() const noexcept;

	// setters
	void setSummary(PlayerSummary summary);
	void setOwnedGames(OwnedGames games);
	void setRecentlyPlayedGames(RecentlyPlayedGames games);	

	// to string
	std::string toString();

private:
	std::string steamId_;

	std::optional<PlayerSummary> summary_;
	std::optional<OwnedGames> ownedGames_;
	std::optional<RecentlyPlayedGames> recentlyPlayedGames_;
};
