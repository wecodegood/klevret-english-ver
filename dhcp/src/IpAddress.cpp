#include "IpAddress.hpp"
#include <stdexcept>
#include <sstream>

IpAddress::IpAddress(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end){
    auto iter = begin;
    int  i = 0;
    while (iter != end && i < IP_ADDRESS_LENGTH){
        _data.at(i) = *iter;
        ++iter;
        ++i;
    }
    if (i < IP_ADDRESS_LENGTH){
        throw std::runtime_error("Не хватает октетов для IP адреса");
    }
}


IpAddress::IpAddress(uint32_t ip_addr){
    _data.at(3) = ip_addr & 0xFFu;
    _data.at(2) = (ip_addr >> 8) & 0xFFu;
    _data.at(1) = (ip_addr >> 16) & 0xFFu;
    _data.at(0) = (ip_addr >> 24) & 0xFFu;
}


std::string IpAddress::to_string() const{
    std::stringstream ss;
    ss  << std::to_string(_data.at(0)) << "."
        << std::to_string(_data.at(1)) << "."
        << std::to_string(_data.at(2)) << "."
        << std::to_string(_data.at(3));
    return ss.str();
}
