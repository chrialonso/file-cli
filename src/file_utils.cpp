#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include "errors.hpp"

[[nodiscard]]
FileError checkFile(const std::string &filename){
    if(filename.empty()) return FileError:: NoFileName;
    bool exists = std::filesystem::exists(filename);
    if(!exists) return FileError::FileNotFound;
    return FileError::Ok;
}

[[nodiscard]]
FileError createFile(const std::string &filename){
    if(filename.empty()) return FileError::NoFileName;
    std::error_code ec;
    std::filesystem::path filepath(filename);

    if(std::filesystem::exists(filepath, ec)){
        return FileError::FileExists;
    }
    if(ec) return FileError::CreateFailure;

    std::ofstream create(filename);
    if(!create){
        ec = std::make_error_code(static_cast<std::errc>(errno));
        if(ec == std::errc::permission_denied) return FileError::PermissionDenied;
        if(ec == std::errc::no_space_on_device) return FileError::DiskFull;
        return FileError::CreateFailure;
    }
    return FileError::Ok;
}

[[nodiscard]]
FileError deleteFile(const std::string &filename){
    FileError checkResult = checkFile(filename);
    if(checkResult != FileError::Ok) return checkResult;

    std::error_code ec;
    if(!std::filesystem::remove(filename, ec)){
        if(ec == std::errc::permission_denied) return FileError::PermissionDenied;
        if(ec == std::errc::no_such_file_or_directory) return FileError::FileNotFound;
        return FileError::UnknownError;
    }
    return FileError::Ok;
}

[[nodiscard]]
FileError writeFile(const std::string &filename, const std::string &input){
    FileError checkResult = checkFile(filename);
    if(checkResult != FileError::Ok) return checkResult;

    std::error_code ec;
    auto perms = std::filesystem::status(filename, ec).permissions();
    if(!ec && (perms & std::filesystem::perms::owner_write) == std::filesystem::perms::none){
        return FileError::PermissionDenied;
    }
    std::ofstream outFile(filename, std::ios::app);
    if(!outFile) return FileError::WriteFailure;
    outFile << input << std::endl;
    if(!outFile) return FileError::WriteFailure;

    return FileError::Ok;
}

[[nodiscard]]
std::pair<std::ifstream, FileError> openFileForReading(const std::string &filename){
    FileError checkResult = checkFile(filename);
    if(checkResult != FileError::Ok) return {std::ifstream(), checkResult};

    std::ifstream file(filename);
    if(!file.is_open()){
        std::error_code ec;
        ec = std::make_error_code(static_cast<std::errc>(errno));
        if(ec == std::errc::permission_denied) return {std::ifstream(), FileError::PermissionDenied};
        return {std::ifstream(), FileError::OpenFailure};
    }
    return {std::move(file), FileError::Ok};
}

[[nodiscard]]
std::pair<std::vector<std::string>, FileError> readFileLines(const std::string& filename){
    FileError checkResult = checkFile(filename);
    if(checkResult != FileError::Ok) return {std::vector<std::string>(), checkResult};
    
    auto [file, err] = openFileForReading(filename);
    if(err != FileError::Ok) return {std::vector<std::string>(), err};

    std::vector<std::string> lines;
    std::string line;
    while(std::getline(file, line)){
        lines.push_back(line);
    }

    if(lines.empty()) return {std::vector<std::string>(), FileError::EmptyFile};
    return {lines, FileError::Ok};
}

[[nodiscard]]
FileError listDirFiles(const std::filesystem::path &path){
    std::cout << "\n--- All Files ---\n";
    std::error_code ec;

    if(!std::filesystem::exists(path,ec)){
        if(ec) return FileError::UnknownError;
        return FileError::PathNotFound;
    } 

    if(!std::filesystem::is_directory(path,ec)){
        if(ec) return FileError::UnknownError;
        return FileError::NotADirectory;
    }

    std::filesystem::directory_iterator dirIt(path, ec);
    if(ec){
        if(ec == std::errc::permission_denied) return FileError::PermissionDenied;
        return FileError::UnknownError;
    }

    for(const auto &entry : dirIt){
        std::cout << entry.path().filename().string() << "\n";
    }
    return FileError::Ok;
}
