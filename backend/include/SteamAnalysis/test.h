#pragma once

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <drogon/drogon.h>
#include <curl/curl.h>

void test(); 

void jsonTest();

void drogonTest();

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);

int curlTest();
