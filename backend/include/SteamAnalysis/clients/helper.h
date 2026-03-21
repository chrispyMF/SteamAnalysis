#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);
std::string buildQuery(const std::string& key, const std::string& value);

// temporary way to get API key until we deploy
std::filesystem::path findEnvFile();
std::string loadEnvFile(const std::string& file);

