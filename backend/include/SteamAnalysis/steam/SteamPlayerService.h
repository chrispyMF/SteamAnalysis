#pragma once

#include <string>
#include <SteamAnalysis/steam/SteamWebApiClient.h>
#include <SteamAnalysis/steam/models/SteamPlayer.h>

/// Class that makes API requests and stores information
/// in a SteamPlayer object.
/// @param apiClient SteamApiClient object to make API requests.
class SteamPlayerService {
public:
	explicit SteamPlayerService(SteamApiClient& apiClient);

	/// Stores information of the API request results into a SteamPlayer object.
	/// @param steamId Steam ID string.
	/// @return SteamPlayer object.
	SteamPlayer loadPlayer(const std::string& steamId);
private:
	SteamApiClient& apiClient_;
};