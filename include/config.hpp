#pragma once
#include <cstddef>
#include <cstdint>
#include <string>

constexpr std::uintmax_t KB = 1024;
constexpr std::uintmax_t MB = 1024 * KB;
constexpr std::uintmax_t GB = MB * 1024;

constexpr size_t MAX_INPUT_LENGTH = 250;
constexpr size_t BINARY_CHECK_BUFFER_SIZE = 512;

struct ParsedArg{
    std::string command;
    std::string value;
    std::string unit;
    bool hasValue = true;
};

struct SearchConfig{
    std::uintmax_t maxFileSize = MB * 5;
    std::size_t maxGlobalMatches = 500;
    std::size_t maxMatchesPerFile = 500;
    int maxDepth = -1;
};
