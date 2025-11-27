#include <iostream>
#include <string>
#include <sstream>
#include "config.hpp"
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
    if(arg.length() > MAX_INPUT_LENGTH) return {arg, InputError::InputTooLong};
    return {arg, InputError::Ok};
}

void handleEditCommand(const std::string &filename){
    std::cout << "\nEditing file: '" << filename << "'\nEnter !exit to exit.\n";
    std::string edit;
    while(true){
        std::cout << "> ";
        if(!std::getline(std::cin, edit)){
            std::cerr << "[ERROR] Input stream or EOF reached.\n";
            break;
        }

        InputError err = parseInput(edit);
        if(!handleInputError(err)) continue;
        if(edit == "!exit"){
            std::cout << "[INFO] Exiting edit mode.\n";
            break;
        }

        FileError writeResult = writeFile(filename, edit);
        if(!handleFileError(writeResult)) break;
    }
}

Command matchCommand(const std::string &input){
    std::istringstream iss(input);
    std::string command;
    iss >> command;

    if(command == "exit") return Command::Exit;

    if(command == "help"){
        std::string helpSearch;
        iss >> helpSearch;
        if(helpSearch == "search") return Command::FlagDetails;
        return Command::Help;
    }

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
    std::cout << "search [pattern] [flags] - Search for content in files matching pattern. Use 'help search' for flag details.\n";
    std::cout << "delete [file name] - Delete file.\n";
}

void showFlagDetails(){
    std::cout << "\n--- Search Command ---\n";
    std::cout << "Usage: search [pattern] [flags]\n\n";
    std::cout << "Searches for content in files matching the given regex pattern.\n\n";
    std::cout << "Optional Flags:\n";
    std::cout << "  --max-file-size=<size><unit>       Maximum file size to search (requires unit: KB, MB, GB)\n";
    std::cout << "                                     Default: 5MB\n\n";
    std::cout << "  --max-global-matches=<number>      Maximum total matches across all files\n";
    std::cout << "                                     Default: 500\n\n";
    std::cout << "  --max-matches-per-file=<number>    Maximum matches per individual file\n";
    std::cout << "                                     Default: 500\n\n";
    std::cout << "  --max-depth=<number>               Maximum directory depth to recurse\n";
    std::cout << "                                     Default: unlimited (-1)\n\n";
    std::cout << "Examples:\n";
    std::cout << "  search hello                                        Search for 'hello' with default settings\n";
    std::cout << "  search myFunction() --max-file-size=1MB             Search with 1MB file size limit\n";
    std::cout << "  search TODO --max-depth=2 --max-global-matches=10\n";
}
