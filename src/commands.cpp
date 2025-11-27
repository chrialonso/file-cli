#include <iostream>
#include "errors.hpp"
#include "input_utils.hpp"
#include "file_utils.hpp"
#include "regex_utils.hpp"
#include "flag_utils.hpp"

void executeCommand(const Command& cmd, const std::string& input){
        switch(cmd){
            case Command::Exit: break;
            case Command::Help:
                showCommands();
                break;
            case Command::Add:{
                auto [file, err] = parseCommand(input);
                if(!handleInputError(err)) break;

                handleEditCommand(file);
                break;
                                }
            case Command::FlagDetails:{
                showFlagDetails();
                break;
                                      }
            case Command::Read:{
                auto [file, inputErr] = parseCommand(input);
                if(!handleInputError(inputErr)) break;

                std::cout << "[INFO] Reading file: '" << file << "'\n";
                auto [lines, fileErr] = readFileLines(file);
                if(!handleFileError(fileErr)) break;

                for(size_t i = 0; i < lines.size(); ++i){
                    std::cout << (i + 1) << ": " << lines[i] << "\n";
                }

                std::cout << "[INFO] Total lines: " << lines.size() << "\n";
                break;
                                }
            case Command::Create:{
                auto [file, err] = parseCommand(input);
                if(!handleInputError(err)) break;

                FileError createResult = createFile(file);
                if(handleFileError(createResult)){
                    std::cout << "[INFO] File '" << file << "' created.\n";
                }
                break;
                                 }
            case Command::List:{
                FileError listResult = listDirFiles();
                if(!handleFileError(listResult)) break;
                               }
            case Command::Find:{
                auto [query, inputErr] = parseCommand(input);
                if(!handleInputError(inputErr)) break;

                auto [re, regErr] = compileRegex(query);
                if(!handleRegexError(regErr)) break;

               auto [files,findErr]  = findFilesByName(re);
                if(!handleRegexError(findErr)) break;

                for(const auto &filepath : files){
                    std::cout << filepath << "\n";
                }
                break;
                               }
            case Command::Search:{
                std::string_view querySV = extractQuery(input);
                std::string query = std::string(querySV);
                std::string_view params = skipWords(input);

                SearchConfig config;

                if(!params.empty()){
                    auto tokens = tokenize(params);

                    auto [parsedArgs, flagErr] = splitFlag(tokens);
                    if(!handleFlagError(flagErr)) break;

                    bool flagError = false;
                    for(const auto& arg : parsedArgs){
                        FlagError applyFlagResult = applyFlag(arg, config);
                        if(!handleFlagError(applyFlagResult)){
                            flagError = true;
                            break;
                        }
                    }
                    if(flagError) break;
                }

                auto [re, regErr] = compileRegex(query);
                if(!handleRegexError(regErr)) break;

                RegexError res = findInFile(query, re, config);
                if(!handleRegexError(res)) break;

                break;
                                 }
            case Command::Delete:{
                auto [file, err] = parseCommand(input);
                if(!handleInputError(err)) break;

                FileError deleteResult = deleteFile(file);
                if(handleFileError(deleteResult)){
                    std::cout << "[INFO] File '" << file << "' deleted.\n";
                }
                break;
                                 }
            case Command::InvalidCommand:
                std::cout << "[ERROR] Invalid command.\n";
                break;
        };
}
