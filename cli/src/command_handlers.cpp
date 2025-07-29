#include "command_handlers.hpp"

#include <map>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>
////
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
////

using namespace std::string_literals;

std::map<KlevretComponent, int> klevret_components{
    {KlevretComponent::CORE, 40236},
};

void check_args_size(const std::stack<CommandElementRealValue>& cmd, int number_of_args, const std::string cmd_str){
    if (cmd.size() != number_of_args){
        throw std::runtime_error("Внутренняя ошибка обработки команды " + cmd_str);
    }
}

void send_cmd(KlevretComponent klevret_component, const  boost::property_tree::ptree& obj){
    //ToDo прицепить компонент
    std::stringstream ss;
    boost::property_tree::write_json(ss, obj);
    std::cout << ss.str() << "\n";
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1){
        throw std::runtime_error("не удалось создать клиентский сокет");
    }
    sockaddr_in sa;
    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(klevret_components[KlevretComponent::CORE]);
    if (inet_pton(AF_INET, "127.0.0.1", &sa.sin_addr) == -1){
        throw std::runtime_error("не удалось преобразовать IP адрес для sockaddr_in");
    }
    if (connect(client_socket, (sockaddr*)&sa, sizeof sa) == -1){
        throw std::runtime_error("не удалось выполнить подключение");
    }
    int sent_bytes = send(client_socket, ss.str().c_str(), ss.str().length(), 0);
    std::cout << "отправлено байт: " << sent_bytes << "\n";
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
    json.put("component", "dhcp");
    json.put("cmd", "ip.address");
    json.put("ip", ip.to_string());
    send_cmd(KlevretComponent::CORE, json);
}

void cmd_dhcp_pool_create(std::stack<CommandElementRealValue>& cmd){
    check_args_size(cmd, 5, "dhcp pool create <IPv4Address> <IPv4Address>");
    IPv4Address ip_end = std::get<IPv4Address>(cmd.top());
    cmd.pop();
    IPv4Address ip_start = std::get<IPv4Address>(cmd.top());
    boost::property_tree::ptree json;
    json.put("component", "dhcp");
    json.put("cmd", "dhcp.pool.create");
    json.put("ip_start", ip_start.to_string());
    json.put("ip_end", ip_end.to_string());
    send_cmd(KlevretComponent::CORE, json);
}
