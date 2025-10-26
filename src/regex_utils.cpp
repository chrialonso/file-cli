#include <iostream>
#include <regex>
#include <filesystem>
#include <fstream>
#include "errors.hpp"

[[nodiscard]]
std::pair<std::regex, RegexError> compileRegex(const std::string &pattern){
    if(pattern.empty()) return {std::regex(), RegexError::EmptyPattern};
    if(pattern.size() > 250) return {std::regex(), RegexError::InputTooLong};

    try{
        std::regex re(pattern, std::regex::ECMAScript | std::regex::icase);
        return {re, RegexError::Ok};
    }catch(const std::regex_error &e){
        std::cerr << "[ERROR] " << e.what() << "\n";
        return {std::regex(), RegexError::InternalRegexError};
    }
}

[[nodiscard]]
std::pair<std::vector<std::string>, RegexError> findFilesByName(const std::regex &re, const std::filesystem::path &start = std::filesystem::current_path()){
    std::vector<std::string> matchingFiles;
    std::error_code ec;

    for(std::filesystem::recursive_directory_iterator it(start, ec); it!= std::filesystem::recursive_directory_iterator(); it.increment(ec)){
        if(ec){
            if(ec == std::errc::permission_denied){
                ec.clear();
                continue;
            }else{
                std::cerr << "[ERROR] Filesystem error while iterating: " << ec.message() << "\n";
                return {matchingFiles, RegexError::UnknownError};
            }
        }

        const std::filesystem::directory_entry &entry = *it;

        std::error_code stat_ec;
        if(!entry.is_regular_file(stat_ec)){
            if(stat_ec == std::errc::permission_denied) continue;
            continue;
        }

        std::string filename = entry.path().filename().string();
        if(std::regex_search(filename, re)){
            matchingFiles.push_back(entry.path().string());
        }
    }

    if(matchingFiles.empty()) return {matchingFiles, RegexError::NoFileFound};
    return {matchingFiles, RegexError::Ok};
}

[[nodiscard]]
RegexError findInFile(const std::string &pattern, const std::regex &re, const std::filesystem::path &start = std::filesystem::current_path()){
    if(pattern.empty()) return RegexError::EmptyPattern;

    std::error_code ec;
    bool found =  false;

    for(std::filesystem::recursive_directory_iterator it(start, ec); it != std::filesystem::recursive_directory_iterator(); it.increment(ec)){
        if(ec){
            if(ec == std::errc::permission_denied){
                ec.clear();
                continue;
            }else{
                std::cerr << "[ERROR] Filesystem error while iteration: " << ec.message() << "\n";
                return RegexError::UnknownError;
            }
        }

        const std::filesystem::directory_entry &entry = *it;

        std::error_code stat_ec;
        if(!entry.is_regular_file(stat_ec)){
            if(stat_ec == std::errc::permission_denied) continue;
            continue;
        }
        
        std::ifstream inFile(entry.path());
        if(!inFile) continue;

        std::string line;
        size_t lineNo= 0;
        bool printedHeaderForFile = false;
        while(std::getline(inFile, line)){
            ++lineNo;
            if(std::regex_search(line, re)){
                found = true;
                if(!printedHeaderForFile){
                    std::cout << "\n" << entry.path().string() << "\n";
                    printedHeaderForFile = true;
                }
                std::cout << lineNo << ": " << line << "\n";
            }
        }
    }
    if(!found) return RegexError::NotInFiles;
    return RegexError::Ok;
}
