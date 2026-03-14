#include <iostream>
#include <filesystem>
#include <SteamAnalysis/clients/SteamWebApiClient.h>
#include <SteamAnalysis/clients/helper.h>

int main() {
	CurlHttpClient client;
	client.get("https://httpbin.org/get");

	return 0;
}
