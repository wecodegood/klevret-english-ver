#pragma once

#include <variant>
#include <string>
#include <vector>
#include "WrapperForParsing.hpp"
#include <boost/property_tree/ptree.hpp>

enum class CommandElementType{
    NONE,
    FIXED_WORD,
    STRING,
    NAME,
    IP_V4_ADDRESS,
    IP_V4_SUBNET_MASK,
    IP_V6_ADDRESS,
    IP_V6_SUBNET_MASK
};


struct CommandElement{
    CommandElement():type(CommandElementType::NONE), fixed_value(""){}
    CommandElement(CommandElementType type, const std::string& fixed_value);
    bool operator==(const CommandElement& another)const=default;

    CommandElementType type;
    std::string fixed_value;
};

std::string to_string(const CommandElement& command_element);

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

struct CommandTree{
    CommandTree()=default;
    CommandElement command_element;
    std::vector<CommandTree> childs;
};

std::vector<Command> get_all_commands();
CommandTree create_command_tree();
