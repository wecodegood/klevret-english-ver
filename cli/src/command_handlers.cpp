#include "command_handlers.hpp"

#include <map>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

using namespace std::string_literals;

enum class KlevretComponent{
    DHCP
};

std::map<KlevretComponent, int> klevret_components{
    {KlevretComponent::DHCP, 65236},
};

void check_args_size(const std::stack<CommandElementRealValue>& cmd, int number_of_args, const std::string cmd_str){
    if (cmd.size() != number_of_args){
        throw std::runtime_error("Внутренняя ошибка обработки команды " + cmd_str);
    }
}

void send_cmd(KlevretComponent klevret_component, const  boost::property_tree::ptree& obj){
    std::stringstream ss;
    boost::property_tree::write_json(ss, obj);
    std::cout << ss.str() << "\n";
}

void cmd_version(std::stack<CommandElementRealValue>& cmd){
    Console::Instance().write_str("Клеврет. Версия 0.1 (в разработке)\n");
}

void blank(std::stack<CommandElementRealValue>& cmd){
    Console::Instance().write_str("пустая команда\n");
}

void cmd_ip_address_ipv4(std::stack<CommandElementRealValue>& cmd){
    check_args_size(cmd, 3, "ip address <IPv4Address>");
    IPv4Address ip = std::get<IPv4Address>(cmd.top());
    boost::property_tree::ptree json;
    json.put("cmd", "ip.address");
    json.put("ip", ip.to_string());
    send_cmd(KlevretComponent::DHCP, json);
}
