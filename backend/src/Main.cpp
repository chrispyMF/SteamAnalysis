#include <iostream>
#include <filesystem>
#include <SteamAnalysis/clients/SteamWebApiClient.h>
#include <SteamAnalysis/models/SteamPlayer.h>
#include <SteamAnalysis/services/SteamPlayerService.h>
#include <SteamAnalysis/logging/Logger.h>
#include <drogon/HttpAppFramework.h>

int main() {
	drogon::app().addListener("0.0.0.0", 80);	// localhost temporarily
	drogon::app().run();
	return 0;
}