#pragma once

#include <array>
#include <cstdint>
#include <string>

class IPv4Address{
public:
    IPv4Address();
    IPv4Address(const std::string& ip);
private:
    std::array<uint8_t, 4> _data;
};

class IPv6Address{
public:
    IPv6Address();
    IPv6Address(const std::string& ip);
private:
    std::array<uint8_t, 6> _data;
};

class IPv4SubnetMask{
public:
    IPv4SubnetMask();
    IPv4SubnetMask(const std::string& mask);
private:
    std::array<uint8_t, 4> _data;
};

class IPv6SubnetMask{
public:
    IPv6SubnetMask();
    IPv6SubnetMask(const std::string& mask);
private:
    std::array<uint8_t, 6> _data;
};
