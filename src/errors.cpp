#include <iostream>
#include "errors.hpp"

void matchInputError(InputError err){
    switch(err){
        case InputError::Ok:
            break;
        case InputError::EmptyField:
            std::cerr << "[ERROR] Input cannot be empty.\n";
            break;
        case InputError::NoInput:
            std::cerr << "[ERROR] No input or EOF reached.\n";
            break;
        case InputError::InputTooLong:
            std::cerr << "[ERROR] Input is too long.\n";
            break;
        case InputError::UnknownInputError:
            std::cerr << "[ERROR] Unknown input error.\n";
            break;
    }
}

void matchFileError(FileError err){
    switch(err){
        case FileError::Ok:
            break;
        case FileError::OpenFailure:
            std::cerr << "[ERROR] Failed to open file.\n";
            break;
        case FileError::ReadFailure:
            std::cerr << "[ERROR] Failed to read file.\n";
            break;
        case FileError::WriteFailure:
            std::cerr << "[ERROR] Failed to write file.\n";
            break;
        case FileError::CreateFailure:
            std::cerr << "[ERROR] Failed to create file.\n";
            break;
        case FileError::DeletionFailure:
            std::cerr << "[ERROR] Failed to delete file.\n";
            break;
        case FileError::FileExists:
            std::cerr << "[ERROR] File already exists.\n";
            break;
        case FileError::EmptyFile:
            std::cerr << "[WARNING] File is empty.\n";
            break;
        case FileError::NoFileName:
            std::cerr << "[ERROR] File has no name.\n";
            break;
        case FileError::FileNotFound:
            std::cerr << "[ERROR] Failed to find file.\n";
            break;
         case FileError::PermissionDenied:
            std::cerr << "[ERROR] Permission denied.\n";
            break;           
        case FileError::DiskFull:
            std::cerr << "[ERROR] Disk is full.\n";
            break;
        case FileError::PathNotFound:
            std::cerr << "[ERROR] The specified path does not exist.\n";
            break;
        case FileError::NotADirectory:
            std::cerr << "[ERROR] The specified path is not a directory.\n";
            break;
         case FileError::UnknownError:
            std::cerr << "[ERROR] Unknown error.\n";
            break;           
    }
}

void matchRegexError(RegexError err){
    switch(err){
        case RegexError::Ok:
            break;
        case RegexError::InvalidRegex:
            std::cerr << "[ERROR] Invalid regex.\n";
            break;
        case RegexError::InvalidInput:
            std::cerr << "[ERROR] Invalid input.\n";
            break;
        case RegexError::UnknownError:
            std::cerr << "[ERROR] Unknown regex error.\n";
            break;
        case RegexError::EmptyPattern:
            std::cerr << "[ERROR] Find pattern is empty.\n";
            break;
        case RegexError::InputTooLong:
            std::cerr << "[ERROR] Input is too long.\n";
            break;
        case RegexError::NotInFiles:
            std::cerr << "[ERROR] Pattern not found in files.\n";
            break;
        case RegexError::NoFileFound:
            std::cerr << "[ERROR] No file found with given pattern.\n";
            break;
        case RegexError::InternalRegexError:
            break;
    }
}

bool handleFileError(FileError err){
    if(err != FileError::Ok){
        matchFileError(err);
        return false;
    }
    return true;
}

bool handleInputError(InputError err){
    if(err != InputError::Ok){
        matchInputError(err);
        return false;
    }
    return true;
}

bool handleRegexError(RegexError err){
    if(err != RegexError::Ok){
        matchRegexError(err);
        return false;
    }
    return true;
}
