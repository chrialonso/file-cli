#pragma once

enum class FileError{
    Ok,
    OpenFailure,
    ReadFailure,
    WriteFailure,
    CreateFailure,
    DeletionFailure,
    FileExists,
    EmptyFile,
    NoFileName,
    FileNotFound,
    PermissionDenied,
    DiskFull,
    PathNotFound,
    NotADirectory,
    UnknownError,
};

enum class InputError{
    Ok,
    EmptyField,
    NoInput,
    InputTooLong,
    UnknownInputError,
};

enum class RegexError{
    Ok,
    InvalidInput,
    InvalidRegex,
    EmptyPattern,
    InputTooLong,
    NotInFiles,
    NoFileFound,
    InternalRegexError,
    UnknownError,
};

void matchFileError(FileError err);
void matchInputError(InputError err);
void matchRegexError(RegexError err);

bool handleFileError(FileError err);
bool handleInputError(InputError err);
bool handleRegexError(RegexError err);
