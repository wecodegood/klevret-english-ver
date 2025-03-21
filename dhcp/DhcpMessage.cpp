#include "DhcpMessage.h"
#include <stdexcept>

DhcpMessage::DhcpMessage()
    :   op(DhcpMessageType::BOOTREQUEST),
        htype(HardwareAddressType::Ethernet_10Mb),
        hlen(get_hardware_address_length(htype)),
        hops(0),
        xid(0),
        secs(0),
        flags(0),
        ciaddr(0),
        yiaddr(0),
        siaddr(0),
        giaddr(0),
        chaddr(nullptr),
        sname(),
        file()
{

}

DhcpMessage::DhcpMessage(std::vector<uint8_t> data)
    :   DhcpMessage()
{
    if (data.size() < DHCP_MESSAGE_MIN_LENGTH){
        throw std::runtime_error("минимальная длина DHCP сообщения: " + std::to_string(DHCP_MESSAGE_MIN_LENGTH));
    }
    op = static_cast<DhcpMessageType>(data.at(0));
    htype = static_cast<HardwareAddressType>(data.at(1));
    hlen = data.at(2);
    hops = data.at(3);
    xid = (static_cast<uint32_t>(data.at(4)) << 24)
        | (static_cast<uint32_t>(data.at(5)) << 16)
        | (static_cast<uint32_t>(data.at(6)) << 8)
        | (static_cast<uint32_t>(data.at(7)));
    secs = (static_cast<uint16_t>(data.at(8)) << 8)
        | (static_cast<uint16_t>(data.at(9)));
    flags = (static_cast<uint16_t>(data.at(10)) << 8)
        | (static_cast<uint16_t>(data.at(11)));
    auto iter = data.begin();
    iter += 11 + 1;
    ciaddr = IpAddress(iter, iter + IP_ADDRESS_LENGTH);
    iter += IP_ADDRESS_LENGTH;
    yiaddr = IpAddress(iter, iter + IP_ADDRESS_LENGTH);
    iter += IP_ADDRESS_LENGTH;
    siaddr = IpAddress(iter, iter + IP_ADDRESS_LENGTH);
    iter += IP_ADDRESS_LENGTH;
    giaddr = IpAddress(iter, iter + IP_ADDRESS_LENGTH);
    iter += IP_ADDRESS_LENGTH;
    if (htype != HardwareAddressType::Ethernet_10Mb){
        throw std::runtime_error("Неподдерживаемый тип адреса");
    }
    chaddr = new MacAddress(iter, iter + MacAddress::address_length);
    iter += DHCP_HARDWARE_ADDRESS_MAX_LENGTH;
    sname = std::basic_string<uint8_t>(iter, iter + 64);
    iter += 64;
    file = std::basic_string<uint8_t>(iter, iter + 128);
    iter += 128;
    // дальше опции
    if (data.size() < DHCP_MESSAGE_MIN_LENGTH + 4){
        return;
    }
    // проверяем на magic cookie 99, 130, 83, 99 (RFC 2131 стр 13 п 3)
    if (*iter != 99 || *(iter + 1) != 130 || *(iter + 2) != 83 || *(iter + 3) != 99){
        throw std::runtime_error("неверное значение magic cookie");
    }
    iter += 4;
    while (iter != data.end()){
        int option_code = *iter;
        ++iter;
        if (option_code == 0 || option_code == 255){
            continue;
        }
        int64_t payload_length = *iter;
        ++iter;
        // ToDo remove try catch
        try{
            DhcpOption new_option(option_code, payload_length, iter, iter + payload_length);
            options.push_back(new_option);
        } catch (...){

        }
        iter += payload_length;
    }
}

std::vector<uint8_t> DhcpMessage::to_network_data(){

}
