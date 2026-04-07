# pragma once

#include <string>
#include <optional>
#include <vector>
#include <utility>
#include <SteamAnalysis/steam/SteamWebApiClient.h>

/// Struct to store response from GetPlayerSummaries method from Steam API.
struct PlayerSummary {
	std::string steamId = "";
	std::string personaName = "";	// Steam username
	std::string profileUrl = "";
	std::string avatar = "";		// Steam profile picture
	std::string avatarMedium = "";
	std::string avatarFull = "";
	std::string locCountryCode = "";
};

/// Struct that represents one Steam game.
/// Utilized by OwnedGames and RecentlyPlayedGames structs.
struct Game {
	std::string name = "";
	std::string imgIconUrl = "";
	int appId = 0;
	int playtimeForever = 0;
	int playtimeTwoWeeks = 0;
};

/// Struct to store response from GetOwnedGames method from Steam API.
struct OwnedGames {
	int gameCount = 0;
	std::vector<Game> gameList = {};
};

/// Struct to store response from GetRecentlyPlayedGames method from Steam API.
struct RecentlyPlayedGames {
	int gameCount = 0;
	std::vector<Game> gameList = {};
};

/// Class that represents a Steam user.
/// Combines information obtained from GetPlayerSummaries, 
/// GetOwnedGames, and GetRecentlyPlayedGames methods from Steam API.
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

	std::string toString();

private:
	std::string steamId_;

	std::optional<PlayerSummary> summary_;
	std::optional<OwnedGames> ownedGames_;
	std::optional<RecentlyPlayedGames> recentlyPlayedGames_;
};
