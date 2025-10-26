#pragma once
#include <string>
#include "errors.hpp"

enum class Command{
    Exit,
    Help,
    Read,
    Create,
    Add,
    Find,
    Search,
    Delete,
    List,
    InvalidCommand,
};

InputError parseInput(const std::string& input);
std::pair<std::string, InputError> parseCommand(const std::string& input);
Command matchCommand(const std::string& input);
void showCommands();
void handleEditCommand(const std::string& filename);
