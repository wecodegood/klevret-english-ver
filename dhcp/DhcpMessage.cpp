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
}

std::vector<uint8_t> DhcpMessage::to_network_data(){

}
