#pragma once

#include <variant>
#include <string>
#include <vector>
#include "WrapperForParsing.hpp"

enum class CommandElementType{
    FIXED_WORD,
    STRING,
    NAME,
    IP_V4_ADDRESS,
    IP_V4_SUBNET_MASK,
    IP_V6_ADDRESS,
    IP_V6_SUBNET_MASK
};


struct CommandElement{
    CommandElement(CommandElementType type, const std::string& name_value);
    CommandElementType type;
    std::string name_value;
};

enum class Language{
    Russian,
    English
};

struct CommandDescription{
    CommandDescription(Language language, const std::string& pattern_description, const std::string& description);
    Language language;
    std::string pattern_description;
    std::string description;
};

class Command{
public:
    Command(const std::string& pattern, const std::vector<CommandDescription>& descriptions);
    std::vector<CommandElement> _elements;
    std::vector<CommandDescription> _descriptions;
private:
    void parse_pattern(WrapperForParsing& wrap);
    void parse_pattern_element(WrapperForParsing& wrap);
    void parse_fixed_word(WrapperForParsing& wrap);
    void parse_variable_element(WrapperForParsing& wrap);
};

std::vector<Command> get_all_commands();
