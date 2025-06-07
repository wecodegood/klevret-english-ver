#include "command_handlers.hpp"

using namespace std::string_literals;

void cmd_version(std::stack<CommandElementRealValue>& cmd){
    Console::Instance().write_str("Клеврет. Версия 0.1 (в разработке)\n");
}

void blank(std::stack<CommandElementRealValue>& cmd){
    Console::Instance().write_str("пустая команда\n");
}

void cmd_ip_address_ipv4(std::stack<CommandElementRealValue>& cmd){
    if (cmd.size() != 3){
        throw std::runtime_error("Внутренняя ошибка обработки команды ip address <IPv4Address>");
    }
    IPv4Address ip = std::get<IPv4Address>(cmd.top());
    Console::Instance().write_str("Вы ввели IP адресс " + ip.to_string() + "\n"s);
}
