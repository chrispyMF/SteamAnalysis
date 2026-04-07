#include <iostream>
#include <filesystem>
#include <SteamAnalysis/steam/SteamWebApiClient.h>
#include <SteamAnalysis/steam/models/SteamPlayer.h>
#include <SteamAnalysis/steam/SteamPlayerService.h>
#include <SteamAnalysis/logging/Logger.h>

int main() {
	Logger logFile(LogLevel::Debug, findFile("log.txt").string());
	SteamApiClient client(logFile);
	SteamPlayerService service(client);
	SteamPlayer player = service.loadPlayer("");	// place steam id here
	std::cout << player.toString() << std::endl;
	return 0;
}