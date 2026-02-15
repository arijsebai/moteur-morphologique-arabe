#pragma once
#include <string>
#include <vector>

// URL and HTTP utilities
std::string url_decode(const std::string& value);
std::string json_escape(const std::string& value);
std::string json_array(const std::vector<std::string>& items);
std::string get_query_param(const std::string& target, const std::string& key);
std::string content_type_for(const std::string& path);
