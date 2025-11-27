#include <iostream>
#include <string>
#include "input_utils.hpp"
#include "commands.hpp"

int main(){
    std::cout << "\nWelcome to my WIP CLI program! Type 'help' for commands.\n";
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
