#include <iostream>
#include <string>
#include <sstream>
#include "errors.hpp"
#include "input_utils.hpp"
#include "file_utils.hpp"

[[nodiscard]]
InputError parseInput(const std::string &input){
    if(input.empty()) return InputError::EmptyField;
    if(input.length() > 250) return InputError::InputTooLong;
    return InputError::Ok;
}

[[nodiscard]]
std::pair<std::string, InputError> parseCommand(const std::string &input){
    std::string command, arg;
    std::istringstream iss(input);
    iss >> command;

    iss >> std::ws;
    std::getline(iss, arg);
    if(arg.empty()) return {arg, InputError::EmptyField};
    if(arg.length() > 250) return {arg, InputError::InputTooLong};
    return {arg, InputError::Ok};
}

void handleEditCommand(const std::string &filename){
    std::cout << "\nEditing file: '" << filename << "'\nEnter a blank space to exit.\n";
    std::string edit;
    while(true){
        std::cout << "> ";
        if(!std::getline(std::cin, edit)){
            std::cerr << "[ERROR] Input stream or EOF reached.\n";
            break;
        }

        InputError err = parseInput(edit);
        if(!handleInputError(err)) continue;
        if(edit == " ") break;

        FileError writeResult = writeFile(filename, edit);
        if(!handleFileError(writeResult)) break;
    }
}

Command matchCommand(const std::string &input){
    std::istringstream iss(input);
    std::string command;
    iss >> command;

    if(command == "exit") return Command::Exit;
    if(command == "help") return Command::Help;
    if(command == "list") return Command::List;
    if(command == "read") return Command::Read;
    if(command == "create") return Command::Create;
    if(command == "add") return Command::Add;
    if(command == "find") return Command::Find;
    if(command == "search") return Command::Search;
    if(command == "delete") return Command::Delete;
    return Command::InvalidCommand;
}

void showCommands(){
    std::cout << "\n--- Commands ---\n";
    std::cout << "exit - Exit the program.\n";
    std::cout << "list - List all file names in directory.\n";
    std::cout << "read [file name] - Print contents in a file by name.\n";
    std::cout << "create [file name] - Create a new file.\n";
    std::cout << "add [file name] - Append to a file by name.\n";
    std::cout << "find [pattern] - Search for files matching pattern.\n";
    std::cout << "search [pattern] - Search for content in files matching pattern.\n";
    std::cout << "delete [file name] - Delete file.\n";
}
