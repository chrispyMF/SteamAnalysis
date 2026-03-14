#include <SteamAnalysis/test.h>

using json = nlohmann::json;

void test() {
    std::cout << "include headers work!\n";
}

void jsonTest() {
    // create json object
    json j = json::parse(R"(
        {
            "name": "anon",
            "age": 21,
            "list": [
                    {"firstName": "John", "lastName": "Doe"},
                    {"firstName": "Jane", "lastName": "Doe"}
            ]
        } 
    )");
    
    // add new values
    j["new"]["key"]["value"] = { "another", "list" };

    //count elements
    auto s = j.size();
    j["size"] = s;

    // pretty print with 4 space indent
    std::cout << std::setw(4) << j << std::endl;
    std::cout << "nlohmann json library works!\n";
}

void drogonTest() {
    std::cout << "Running Drogon webserver ...\n";
    drogon::app().addListener("0.0.0.0", 8080);
    drogon::app().run();
    std::cout << "Drogon webserver stopped.\n";
}

int curlTest() {
    CURL* curl = curl_easy_init();

    if (!curl) {
        std::cerr << "Failed to initilized CURL\n";
        return 1;
    }

    std::string response;

    curl_easy_setopt(curl, CURLOPT_URL, "https://httpbin.org/get?test=testing");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::cerr << "CURL Error: " << curl_easy_strerror(res) << std::endl;
    }
    else {
        std::cout << "Response:\n" << response << std::endl;
    }

    curl_easy_cleanup(curl);
    return 0;
}
