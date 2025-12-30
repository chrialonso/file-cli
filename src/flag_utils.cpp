#include <cstdint>
#include <string_view>
#include "flag_utils.hpp"
#include "errors.hpp"

std::string_view extractQuery(std::string_view sv){
    size_t spacePos = sv.find(' ');
    if (spacePos == std::string_view::npos) return {};

    sv.remove_prefix(spacePos + 1);
    size_t flagPos = sv.find("--");
    sv = sv.substr(0, flagPos);
    size_t end = sv.find_last_not_of(' ');
    if(end == std::string_view::npos) return {};

    return sv.substr(0, end + 1);
}

std::string_view skipWords(std::string_view sv){
    size_t flagPos = sv.find_first_of("--");
    if(flagPos == std::string_view::npos) return "";

    sv.remove_prefix(flagPos);
    return sv;
}

std::vector<std::string_view> tokenize(std::string_view sv){
    std::vector<std::string_view> tokens;

    while(!sv.empty()){
        size_t space = sv.find(' ');
        if(space == std::string_view::npos){
            tokens.push_back(sv);
            break;
        }

        tokens.push_back(sv.substr(0, space));
        sv.remove_prefix(space + 1);
    }
    return tokens;
}

bool isFlag(const std::string_view& flag){
    if(!(flag.substr(0,2) == "--")){
        return false;
    }
    return true;
}

std::pair<std::vector<ParsedArg>, FlagError> splitFlag(const std::vector<std::string_view>& flags){
    std::vector<ParsedArg> results;
    results.reserve(flags.size());

    for(auto flag : flags){
        ParsedArg arg;
        
        if(!isFlag(flag)) return {{}, FlagError::InvalidFlag};

        flag.remove_prefix(2);

        size_t eqPos = flag.find('=');
        if(eqPos == std::string_view::npos){
            arg.command = flag;
            arg.value = "";
            arg.unit = "";
            arg.hasValue = false;
            results.push_back(arg);
            continue;
        }

        arg.command = flag.substr(0, eqPos);
        flag.remove_prefix(eqPos + 1);
        if(flag.empty()) return {{}, FlagError::EmptyParams};

        size_t unitPos = flag.find_first_not_of("0123456789");

        if(unitPos == std::string_view::npos){
            arg.value = flag;
            arg.unit = "";
            arg.hasValue = true;
            results.push_back(arg);
            continue;
        }

        std::string_view number = flag.substr(0, unitPos);
        if(number.empty()) return {{}, FlagError::NoValue};

        std::string_view unit = flag.substr(unitPos);
        if(unit.empty()) return {{}, FlagError::NoUnit};

        arg.value = number;
        arg.unit = unit;
        arg.hasValue = true;
        results.push_back(arg);
    }
    return {results, FlagError::Ok};
}

FlagError parseNumber(const std::string& num, uintmax_t& out){
    for(char c : num){
        if(!isdigit(c)) return FlagError::InvalidValue;
    }

    try{
        out = std::stoull(num);
    }catch(...){
        return FlagError::InvalidValue;
    }
    return FlagError::Ok;
}

FlagError parseSize(const ParsedArg& arg, uintmax_t& out){
        uintmax_t num = 0;
        FlagError parseNumberResult = parseNumber(arg.value, num);
        if(parseNumberResult != FlagError::Ok) return parseNumberResult;

        if(arg.unit == "KB"){
            out = num * KB;
        }else if(arg.unit == "MB"){
            out = num * MB;
        }else if(arg.unit == "GB"){
            out = num * GB;
        }else{
            return FlagError::InvalidUnit;
        }
        return FlagError::Ok;
}

FlagError applyFlag(const ParsedArg& arg, SearchConfig& config){
    std::string_view cmd = arg.command;

    if(cmd == "max-file-size" || cmd == "mfs"){
        if(!arg.hasValue) return FlagError::Ok;
        if(arg.unit.empty()) return FlagError::NoUnit;

        uintmax_t bytes = 0;
        FlagError parseSizeResult = parseSize(arg, bytes);
        if(parseSizeResult != FlagError::Ok) return parseSizeResult;

        config.maxFileSize = bytes;
        return FlagError::Ok;
    }else if(cmd == "max-global-matches" || cmd == "mgm"){
        if(!arg.hasValue) return FlagError::Ok;
        if(!arg.unit.empty()) return FlagError::UnitNotAllowed; 

        uintmax_t num = 0;
        FlagError parseNumberResult = parseNumber(arg.value, num);
        if(parseNumberResult != FlagError::Ok) return parseNumberResult;

        config.maxGlobalMatches = static_cast<size_t>(num);
        return FlagError::Ok;
    }else if(cmd == "max-matches-per-file" || cmd == "mmpf"){
        if(!arg.hasValue) return FlagError::Ok;
        if(!arg.unit.empty()) return FlagError::UnitNotAllowed;

        uintmax_t num = 0;
        FlagError parseNumResult = parseNumber(arg.value, num);
        if(parseNumResult != FlagError::Ok) return parseNumResult;

        config.maxMatchesPerFile = static_cast<size_t>(num);
        return FlagError::Ok;
    }else if(cmd == "max-depth" || cmd == "md"){
        if(!arg.hasValue) return FlagError::Ok;
        if(!arg.unit.empty()) return FlagError::UnitNotAllowed;

        uintmax_t num = 0;
        FlagError parseNumResult = parseNumber(arg.value, num);
        if(parseNumResult != FlagError::Ok) return parseNumResult;

        config.maxDepth = static_cast<int>(num);
        return FlagError::Ok;
    }
    return FlagError::UnknownFlag;
}
