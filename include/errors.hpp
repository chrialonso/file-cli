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

enum class FlagError{
    Ok,
    EmptyFlag,
    InvalidFlag,
    NoValue,
    NoUnit,
    EmptyParams,
    InvalidValue,
    InvalidUnit,
    UnitNotAllowed,
    UnknownFlag,
};

void matchFileError(FileError err);
void matchInputError(InputError err);
void matchRegexError(RegexError err);
void matchFlagError(FlagError err);

[[nodiscard]] bool handleFileError(FileError err);
[[nodiscard]] bool handleInputError(InputError err);
[[nodiscard]] bool handleRegexError(RegexError err);
[[nodiscard]] bool handleFlagError(FlagError err);
