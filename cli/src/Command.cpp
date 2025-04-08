#include "Command.hpp"

#include "WrapperForParsing.hpp"
#include <stdexcept>
#include <map>



CommandElement::CommandElement(CommandElementType type, const std::string& name_value)
    : type(type), name_value(name_value)
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
    while(std::isalpha(wrap.ch)){
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

};

std::vector<Command> get_all_commands(){
    return all_commands;
}
