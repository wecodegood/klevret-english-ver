#pragma once

#include <variant>
#include <string>
#include <vector>
#include "WrapperForParsing.hpp"
#include <boost/property_tree/ptree.hpp>
#include "ip_addresses.hpp"
#include <stack>

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

using CommandElementRealValue = std::variant<
    std::string,
    IPv4Address,
    IPv6Address,
    IPv4SubnetMask,
    IPv6SubnetMask
>;

class Command;
using command_handler = std::function<void(std::stack<CommandElementRealValue>&)>;

struct CommandElement{
    CommandElement():type(CommandElementType::NONE), fixed_value(""){}
    CommandElement(CommandElementType type, const std::string& fixed_value);
    bool operator==(const CommandElement& another)const{
        return type == another.type && fixed_value == another.fixed_value;
    }

    CommandElementType type;
    std::string fixed_value;
    command_handler handler = nullptr;
};

bool check_command_element(const std::string& str, CommandElement element);

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

enum class KlevretComponent{
    CLI,
    CORE,
    DHCP,
};

class Command{
public:
    Command(KlevretComponent klevret_component, const std::string& pattern,
        const std::vector<CommandDescription>& descriptions, command_handler handler);
    std::vector<CommandElement> elements;
    std::vector<CommandDescription> descriptions;
    const command_handler handler;
    KlevretComponent klevret_component;
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
