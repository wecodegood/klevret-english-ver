#include "Command.hpp"

#include "WrapperForParsing.hpp"
#include <stdexcept>
#include <map>
#include <boost/property_tree/ptree.hpp>


std::string to_string(const CommandElement& command_element){
    switch (command_element.type)
    {
        case CommandElementType::FIXED_WORD: return command_element.fixed_value;
        case CommandElementType::IP_V4_ADDRESS: return "<IPv4 address>";
        case CommandElementType::IP_V4_SUBNET_MASK: return "<subnet mask (IPv4)>";
        case CommandElementType::IP_V6_ADDRESS: return "<IPv6 address>";
        case CommandElementType::IP_V6_SUBNET_MASK: return "<subnet mask (IPv6)>";
        case CommandElementType::NAME: return "<name>";
        case CommandElementType::STRING: return "<string>";
        default: return "<UNKNOWN>";
    }
}

CommandElement::CommandElement(CommandElementType type, const std::string& fixed_value)
    : type(type), fixed_value(fixed_value)
{

}

CommandDescription::CommandDescription(Language language, const std::string& pattern_description, const std::string& description)
    : language(language), pattern_description(pattern_description), description(description)
{

}




void Command::parse_fixed_word(WrapperForParsing& wrap){
    std::string word = "";
    while (std::isalpha(wrap.ch)){
        word += wrap.ch;
        wrap.get_next();
    }
    CommandElement new_command_element(CommandElementType::FIXED_WORD, word);
    _elements.push_back(new_command_element);
}

std::map<std::string, CommandElementType> str_to_element_type_map{
    {"string", CommandElementType::STRING},
    {"name", CommandElementType::NAME},
    {"IPv4Address", CommandElementType::IP_V4_ADDRESS},
    {"IPv4SubnetMask", CommandElementType::IP_V4_SUBNET_MASK},
    {"IPv6Address", CommandElementType::IP_V6_ADDRESS},
    {"IPv6SubnetMask", CommandElementType::IP_V6_SUBNET_MASK},

};

void Command::parse_variable_element(WrapperForParsing& wrap){
    if (wrap.ch != '<'){
        throw std::runtime_error("Ошибка парсинга шаблона команды: ожидалось <");
    }
    wrap.get_next();
    std::string variable_element = "";
    while(std::isalnum(wrap.ch)){
        variable_element += wrap.ch;
        wrap.get_next();
    }
    if (variable_element == ""){
        throw std::runtime_error("Ошибка парсинга шаблона команды: вариативный элемент не может быть пустым <>");
    }
    auto iter = str_to_element_type_map.find(variable_element);
    if (iter == str_to_element_type_map.end()){
        throw std::runtime_error("Ошибка парсинга шаблона команды: неизвестный вариативный элемент <" + variable_element + ">");
    }
    CommandElement new_element(iter->second, "");
    _elements.push_back(new_element);
    if (wrap.ch != '>'){
        throw std::runtime_error("Ошибка парсинга шаблона команды: ожидалось <");
    }
    wrap.get_next();
}

void Command::parse_pattern_element(WrapperForParsing& wrap){
    if (std::isalpha(wrap.ch)){
        parse_fixed_word(wrap);
    } else if(wrap.ch == '<'){
        parse_variable_element(wrap);
    } else {
        throw std::runtime_error("Ошибка парсинга шаблона команды: ожидалось слово или <элемент>");
    }
}

void Command::parse_pattern(WrapperForParsing& wrap){
    while (!wrap.is_end()){
        wrap.skip_whitespaces();
        parse_pattern_element(wrap);
    }
}


Command::Command(const std::string& pattern, const std::vector<CommandDescription>& descriptions)
    :   _descriptions(descriptions)
{
    WrapperForParsing wrap(pattern);
    parse_pattern(wrap);
    if (!wrap.is_end()){
        throw std::runtime_error("Ошибка парсинга шаблона команды: ожидался конец текста");
    }
}



std::vector<Command> all_commands = {
    {
        "version",
        {
            {Language::Russian, "version", "показать версию ПО Клеврет"},
            {Language::English, "version", "show Klevret software version"}
        }
    },
    {
        "ip show",
        {

        }
    },
    {
        "ip address show",
        {

        }
    },
    {
        "ip address <IPv4Address>",
        {

        }
    }

};

std::vector<Command> get_all_commands(){
    return all_commands;
}

void add_command_to_tree(CommandTree& tree_root, const Command& command){
    CommandTree *current_node = &tree_root;
    for (const auto& command_element : command._elements){
        bool found = false;
        for (auto& node : current_node->childs){
            if (node.command_element == command_element){
                found = true;
                current_node = &node;
                break;
            }
        }
        if (!found){
            CommandTree new_node;
            new_node.command_element = command_element;
            current_node->childs.push_back(new_node);
            current_node = &(current_node->childs.at(current_node->childs.size() - 1));
        }
    }
}

CommandTree create_command_tree(){
    CommandTree tree;
    auto all_commands = get_all_commands();
    for (auto& command : all_commands){
        add_command_to_tree(tree, command);
    }
    return tree;
}
