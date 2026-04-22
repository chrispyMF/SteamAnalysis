#include <SteamAnalysis/models/SteamPlayer.h>

SteamPlayer::SteamPlayer(std::string steamId) 
	: steamId_(steamId) {}

const std::string& SteamPlayer::steamId() const noexcept {
	return steamId_;
}

bool SteamPlayer::hasSummary() const noexcept {
	return summary_.has_value();
}

bool SteamPlayer::hasOwnedGames() const noexcept {
	return ownedGames_.has_value();
}

bool SteamPlayer::hasRecentlyPlayedGames() const noexcept {
	return recentlyPlayedGames_.has_value();
}

const std::optional<PlayerSummary>& SteamPlayer::summary() const noexcept {
	return summary_;
}

const std::optional<OwnedGames>& SteamPlayer::ownedGames() const noexcept {
	return ownedGames_;
}

const std::optional<RecentlyPlayedGames>& SteamPlayer::recentlyPlayedGames() const noexcept {
	return recentlyPlayedGames_;
}

void SteamPlayer::setSummary(PlayerSummary summary) {
	summary_ = summary;
}

void SteamPlayer::setOwnedGames(OwnedGames games) {
	ownedGames_ = games;
}

void SteamPlayer::setRecentlyPlayedGames(RecentlyPlayedGames games) {
	recentlyPlayedGames_ = games;
}

std::string SteamPlayer::toString() {
	return	"Steam Username: " + summary().value().personaName +
			"\n\tSteam ID: " + summary().value().steamId +
			"\n\tAvatar: " + summary().value().avatarFull + 
			"\n\tProfile URL: " + summary().value().profileUrl +
			"\n\tGames: " + std::to_string(ownedGames().value().gameCount) + 
			"\n\tRecently Played Games: " + std::to_string(recentlyPlayedGames().value().gameCount);
}
