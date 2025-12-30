#include <iostream>
#include <string>
#include <filesystem>
#include "input_utils.hpp"
#include "commands.hpp"

int main(){
std::cout << "\n--- CLI File Manager v1.0 ---\n";
std::cout << "Current directory: " << std::filesystem::current_path().string() << "\n";
std::cout << "Type 'help' for commands or 'exit' to quit.\n";
    std::string input;

    while(true){
        std::cout << "\n> ";
        
        if(!std::getline(std::cin, input)){
            std::cout << "\nExiting program.\n";
            break;
        }

        const Command inputResult = matchCommand(input);
        if(inputResult == Command::Exit) break;
        executeCommand(inputResult, input);
    }
    return 0;
}
