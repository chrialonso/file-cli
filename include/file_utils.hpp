#pragma once
#include <string>
#include <fstream>
#include <filesystem>
#include "errors.hpp"

FileError checkFile(const std::string& filename);
std::pair<std::ifstream, FileError> openFileForReading(const std::string& filename);
FileError writeFile(const std::string& filename, const std::string &input);
FileError createFile(const std::string& filename);
FileError deleteFile(const std::string& filename);
std::pair<std::vector<std::string>, FileError> readFileLines(const std::string& filename);
FileError listDirFiles(const std::filesystem::path& path = std::filesystem::current_path());
