#include "DhcpMessage.hpp"
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
    ciaddr = IPv4Address::from_big_endian_bytes(iter, iter + IP_V4_ADDRESS_LENGTH);
    iter += IP_V4_ADDRESS_LENGTH;
    yiaddr = IPv4Address::from_big_endian_bytes(iter, iter + IP_V4_ADDRESS_LENGTH);
    iter += IP_V4_ADDRESS_LENGTH;
    siaddr = IPv4Address::from_big_endian_bytes(iter, iter + IP_V4_ADDRESS_LENGTH);
    iter += IP_V4_ADDRESS_LENGTH;
    giaddr = IPv4Address::from_big_endian_bytes(iter, iter + IP_V4_ADDRESS_LENGTH);
    iter += IP_V4_ADDRESS_LENGTH;
    if (htype != HardwareAddressType::Ethernet_10Mb){
        throw std::runtime_error("Неподдерживаемый тип адреса");
    }
    chaddr = std::unique_ptr<HardwareAddress>(MacAddress::make_ptr_from_big_endian_bytes(iter, iter + MacAddress::address_length));
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
        if (option_code == 0){
            continue;
        }
        if (option_code == 255){
            break;
        }
        int64_t payload_length = *iter;
        ++iter;
        DhcpOption new_option(option_code, payload_length, iter, iter + payload_length);
        options.push_back(new_option);
        iter += payload_length;
    }
}

std::vector<uint8_t> DhcpMessage::to_network_data() {
    std::vector<uint8_t> result;
    result.push_back(static_cast<uint8_t>(op));
    result.push_back(static_cast<uint8_t>(htype));
    result.push_back(hlen);
    result.push_back(hops);
    result.push_back((int)(xid >> 24));
    result.push_back((int)((xid >> 16) & 0xFF));
    result.push_back((int)((xid >> 8) & 0xFF));
    result.push_back((int)((xid) & 0xFF));
    result.push_back((int)((secs >> 8) & 0xFF));
    result.push_back((int)((secs) & 0xFF));
    result.push_back((int)((flags >> 8) & 0xFF));
    result.push_back((int)(flags & 0xFF));
    auto ciaddr_data = ciaddr.to_network_data();
    result.insert(result.cend(), ciaddr_data.begin(), ciaddr_data.end());
    auto yiaddr_data = yiaddr.to_network_data();
    result.insert(result.cend(), yiaddr_data.begin(), yiaddr_data.end());
    auto siaddr_data = siaddr.to_network_data();
    result.insert(result.cend(), siaddr_data.begin(), siaddr_data.end());
    auto giaddr_data = giaddr.to_network_data();
    result.insert(result.cend(), giaddr_data.begin(), giaddr_data.end());
    auto chaddr_data = chaddr.get()->to_chaddr();
    result.insert(result.cend(), chaddr_data.begin(), chaddr_data.end());
    std::array<uint8_t, 64> sname_data;
    sname_data.fill(0);
    result.insert(result.cend(), sname_data.begin(), sname_data.end());
    std::array<uint8_t, 128> file_data;
    file_data.fill(0);
    result.insert(result.cend(), file_data.begin(), file_data.end());
    if (options.size() == 0){
        return result;
    }
    result.push_back(99);
    result.push_back(130);
    result.push_back(83);
    result.push_back(99);
    for (auto& option : options){
        auto opt_data = option.to_network_data();
        result.insert(result.cend(), opt_data.begin(), opt_data.end());
    }
    result.push_back(255); // end option
    return result;
}
