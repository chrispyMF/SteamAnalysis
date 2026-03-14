#include <SteamAnalysis/clients/SteamWebApiClient.h>

std::string CurlHttpClient::get(const std::string& url) {
	CURL* curl = curl_easy_init();
	if (!curl) {
		std::cerr << "Failed to initialize CURL\n";
		return "";
	}

	std::string response;
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		std::cerr << "CURL Error: " << curl_easy_strerror(res) << std::endl;
		return "";
	}
	else {
		std::cout << "Response\n" << response << std::endl;
	}

	curl_easy_cleanup(curl);

	return response;
}