#include "HardwareAddress.h"
#include <stdexcept>

uint8_t get_hardware_address_length(HardwareAddressType htype){
    switch (htype)
    {
    case HardwareAddressType::Ethernet_10Mb: return 6;

    }
    return 0;
}


MacAddress::MacAddress(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end){
    int i = 0;
    auto iter = begin;
    while (iter != end && i < address_length){
        data.at(i) = *iter;
        ++iter;
        ++i;
    }
    if (i < address_length){
        throw std::runtime_error("MAC адрес заполнен не полностью");
    }
}

chaddr_t MacAddress::to_chaddr(){
    chaddr_t result;
    for (auto& elem : result){
        elem = 0;
    }
    for (int i = 0; i < DHCP_HARDWARE_ADDRESS_MAX_LENGTH; ++i){
        result.at(i) = data.at(i);
    }
    return result;
}
