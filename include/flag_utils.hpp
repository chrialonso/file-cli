#pragma once
#include "errors.hpp"
#include "config.hpp"
#include <vector>
#include <string>

std::string_view extractQuery(std::string_view sv);

std::string_view skipWords(std::string_view sv);

std::vector<std::string_view> tokenize(std::string_view sv);

bool isFlag(const std::string_view& flag);

std::pair<std::vector<ParsedArg>, FlagError> splitFlag(const std::vector<std::string_view>& flags);

[[nodiscard]] FlagError parseNumber(const std::string& num, uintmax_t& out);
[[nodiscard]] FlagError parseSize(const ParsedArg& arg, uintmax_t& out);
[[nodiscard]] FlagError applyFlag(const ParsedArg& arg, SearchConfig& config);
