#pragma once
#include <regex>
#include <filesystem>
#include "errors.hpp"
#include "config.hpp"

[[nodiscard]] std::pair<std::regex, RegexError> compileRegex(const std::string& pattern);
[[nodiscard]] std::pair<std::vector<std::string>, RegexError> findFilesByName(const std::regex& re, const std::filesystem::path& start = std::filesystem::current_path());
[[nodiscard]] RegexError findInFile(const std::string& pattern, const std::regex& re, SearchConfig& config, const std::filesystem::path& start = std::filesystem::current_path());
