#include <iostream>
#include <filesystem>
#include <SteamAnalysis/clients/SteamWebApiClient.h>
#include <SteamAnalysis/logging/logger.h>
#include <SteamAnalysis/helper.h>

int main() {
	Logger logFile(LogLevel::Info, findFile("log.txt").string());
	SteamApiClient client(logFile);
	std::cout << std::setw(4) << client.getPlayerSummaries("").data << std::endl;
	std::cout << std::setw(4) << client.getGetOwnedGames("").data << std::endl;
	std::cout << std::setw(4) << client.getRecentlyPlayedGames("").data << std::endl;

	return 0;
}