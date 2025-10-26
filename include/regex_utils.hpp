#pragma once
#include <regex>
#include <filesystem>
#include "errors.hpp"

std::pair<std::regex, RegexError> compileRegex(const std::string& pattern);
std::pair<std::vector<std::string>, RegexError> findFilesByName(const std::regex& re, const std::filesystem::path& start = std::filesystem::current_path());
RegexError findInFile(const std::string& pattern, const std::regex& re, const std::filesystem::path& start = std::filesystem::current_path());

