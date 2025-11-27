#pragma once
#include <string>
#include "errors.hpp"

enum class Command{
    Exit,
    Help,
    FlagDetails,
    Read,
    Create,
    Add,
    Find,
    Search,
    Delete,
    List,
    InvalidCommand,
};

[[nodiscard]] InputError parseInput(const std::string& input);
[[nodiscard]] std::pair<std::string, InputError> parseCommand(const std::string& input);
void handleEditCommand(const std::string& filename);
Command matchCommand(const std::string& input);
void showCommands();
void showFlagDetails();
