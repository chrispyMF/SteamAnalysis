#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <regex>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/// Callback used to handle data recived from a network operation.
/// Appends the recived contents to a specified string output and returns the total size of the data processed.
/// @param contents Pointer to the raw data buffer received from the network.
/// @param size Size in bytes of each data element in the buffer. 
/// @param nmemb Number of elements in the data buffer.
/// @param output Pointer to the string that will be appended with the received data. 
///	@return Size of data processed in bytes.
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);

/// Builds a REST API query paramter.
/// @param key String that contains the query name. 
/// @param value String that contains the value of the query.
/// @return String query parameter.
std::string buildQuery(const std::string& key, const std::string& value);

/// Hides API key from the Http request url.
/// @param url Http request url string.
/// @return String url with API key hidden.
std::string sanitizeUrl(const std::string& url);

// temporary way to get API key with .env file until we deploy

/// Finds a file with given file name. Starts searching from root folder of project.
/// @param fileName Name of file as string. 
/// @return std::filesystem::path object that contains path to file.
std::filesystem::path findFile(const std::string& fileName);

/// Opens .env file and gets API key.
/// @param file File path as string. 
/// @return API key as string.
std::string loadEnvFile(const std::string& file);

