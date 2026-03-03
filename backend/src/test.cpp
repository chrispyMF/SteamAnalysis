#include <SteamAnalysis/test.h>
#include <drogon/drogon.h>

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
    drogon::app().addListener("0.0.0.0", 8080).run();
    std::cout << "Drogon webserver stopped.\n";
}