#pragma once

#include <cstdint>
#include "IpAddress.hpp"
#include "HardwareAddress.hpp"
#include <string>
#include "DhcpOption.hpp"
#include <vector>

// RFC 2131, page 10
enum class DhcpMessageType : uint8_t{
    BOOTREQUEST = 1,
    BOOTREPLY = 2
};

constexpr int DHCP_MESSAGE_MIN_LENGTH = 236;

// RFC 2131, page 10
struct DhcpMessage{
    DhcpMessage();
    DhcpMessage(std::vector<uint8_t> data);
    std::vector<uint8_t> to_network_data();

    DhcpMessageType op;
    HardwareAddressType htype;
    uint8_t hlen;
    uint8_t hops;
    uint32_t xid;
    uint16_t secs;
    uint16_t flags;
    IpAddress ciaddr;
    IpAddress yiaddr;
    IpAddress siaddr;
    IpAddress giaddr;
    HardwareAddress* chaddr;
    std::basic_string<uint8_t> sname;
    std::basic_string<uint8_t> file;
    std::vector<DhcpOption> options;
};
