#pragma once

#include <string>
#include <SteamAnalysis/steam/SteamWebApiClient.h>
#include <SteamAnalysis/steam/models/SteamPlayer.h>

class SteamPlayerService {
public:
	explicit SteamPlayerService(SteamApiClient& apiClient);

	SteamPlayer loadPlayer(const std::string& steamId);
private:
	SteamApiClient& apiClient_;
};