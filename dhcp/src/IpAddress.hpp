#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>

constexpr int IP_ADDRESS_LENGTH = 4;

class IpAddress{
public:
    IpAddress(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end);
    IpAddress(uint32_t ip_addr);
    std::string to_string() const;
private:
    std::array<uint8_t, IP_ADDRESS_LENGTH> _data;   // big endian
};

using subnet_mask_t = uint32_t;
