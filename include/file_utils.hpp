#pragma once
#include <string>
#include <fstream>
#include <filesystem>
#include "errors.hpp"

[[nodiscard]] FileError checkFile(const std::string& filename);
[[nodiscard]] std::pair<std::ifstream, FileError> openFileForReading(const std::string& filename);
[[nodiscard]] FileError writeFile(const std::string& filename, const std::string& input);
[[nodiscard]] FileError createFile(const std::string& filename);
[[nodiscard]] FileError deleteFile(const std::string& filename);
[[nodiscard]] std::pair<std::vector<std::string>, FileError> readFileLines(const std::string& filename);
[[nodiscard]] FileError listDirFiles(const std::filesystem::path& path = std::filesystem::current_path());
