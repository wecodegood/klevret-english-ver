#include "IpAddress.hpp"
#include <stdexcept>
#include <sstream>
#include "WrapperForParsing.hpp"
#include <algorithm>


IPv4Address IPv4Address::from_big_endian_bytes(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end){
    IPv4Address ip_address(0);

    auto iter = begin;
    int  i = 0;
    while (iter != end && i < IP_V4_ADDRESS_LENGTH){
        ip_address._data.at(i) = *iter;
        ++iter;
        ++i;
    }
    if (i < IP_V4_ADDRESS_LENGTH){
        throw std::runtime_error("Не хватает октетов для IP адреса");
    }

    return ip_address;
}

int parse_octet(WrapperForParsing &wrap){
    size_t octet = 0;
    while (wrap.ch >= '0' && wrap.ch <= '9'){
        octet = octet * 10 + (wrap.ch - '0');
        wrap.get_next();
    }
    if (octet > 255){
        throw std::runtime_error("Ошибка парсинга IP-адреса, октет не может быть больше 255");
    }
    return octet;
}

IPv4Address::IPv4Address(std::string ip_addr){
    WrapperForParsing wrap(ip_addr);
    for (int i = 0; i < 4; ++i){
        _data.at(i) = parse_octet(wrap);
        if (i < 3 && wrap.ch != '.'){
            throw std::runtime_error("Ошибка парсинга IP-адреса, ожидалась точка");
        }
        wrap.get_next();
    }
    if (wrap.ch != WrapperForParsing::EOT){
        throw std::runtime_error("Ошибка парсинга IP-адреса, неизвестный символ: " + wrap.ch);
    }
}

IPv4Address::IPv4Address(uint32_t ip_addr){
    _data.at(3) = ip_addr & 0xFFu;
    _data.at(2) = (ip_addr >> 8) & 0xFFu;
    _data.at(1) = (ip_addr >> 16) & 0xFFu;
    _data.at(0) = (ip_addr >> 24) & 0xFFu;
}


std::string IPv4Address::to_string() const{
    std::stringstream ss;
    ss  << std::to_string(_data.at(0)) << "."
        << std::to_string(_data.at(1)) << "."
        << std::to_string(_data.at(2)) << "."
        << std::to_string(_data.at(3));
    return ss.str();
}


std::vector<uint8_t> IPv4Address::to_network_data() const{
    std::vector<uint8_t> result;
    for (int i = 0; i < IP_V4_ADDRESS_LENGTH; ++i){
        result.push_back(_data.at(i));
    }
    return result;
}

IPv4SubnetMask IPv4SubnetMask::from_big_endian_bytes(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end){
    IPv4SubnetMask mask(0);
    auto iter = begin;
    int  i = 0;
    while (iter != end && i < IP_V4_SUBNET_MASK_LENGTH){
        mask._data.at(i) = *iter;
        ++iter;
        ++i;
    }
    if (i < IP_V4_SUBNET_MASK_LENGTH){
        throw std::runtime_error("Не хватает октетов для маски подсети");
    }
    return mask;
}

IPv4SubnetMask::IPv4SubnetMask(uint32_t mask){
    _data.at(3) = mask & 0xFFu;
    _data.at(2) = (mask >> 8) & 0xFFu;
    _data.at(1) = (mask >> 16) & 0xFFu;
    _data.at(0) = (mask >> 24) & 0xFFu;
}


IPv4SubnetMask::IPv4SubnetMask(std::string mask){
    WrapperForParsing wrap(mask);
    std::vector<uint8_t> allowed_mask_values = {128, 192, 224, 240, 248, 252, 254, 255};
    uint8_t previous_value = 255;
    for (int i = 0; i < 4; ++i){
        size_t octet = parse_octet(wrap);
        if (std::find(allowed_mask_values.begin(), allowed_mask_values.end(), octet) == allowed_mask_values.end()){
            throw std::runtime_error("Ошибка парсинга маски подсети: недопустимое значение октета");
        }
        if (previous_value != 255){
            throw std::runtime_error("Ошибка парсинга маски подсети: маска должна быть непрерывной цепочкой бит равных 1");
        }
        previous_value = octet;
        if (i != 3){
            if (wrap.ch != '.'){
            throw std::runtime_error("Ошибка парсинга маски подсети: ожидалась точка");
            }
            wrap.get_next();
        }
    }
    if (wrap.ch != WrapperForParsing::EOT){
        throw std::runtime_error("Ошибка парсинга маски подсети: неизвестный символ: " + wrap.ch);
    }
}

IPv4SubnetMask IPv4SubnetMask::from_prefix(int prefix){
    uint32_t one = 128u;
    uint32_t mask = 0;
    if (prefix < 0 || prefix > 32){
        throw std::runtime_error("Неверный префикс (маска подсети). Допустимые значения [0..32]");
    }
    for (int i = 0; i < prefix; ++i){
        mask |= one;
        one >>= 1;
    }
    return IPv4SubnetMask(mask);
}

std::string IPv4SubnetMask::to_string() const{
    std::stringstream ss;
    ss  << std::to_string(_data.at(0)) << "."
        << std::to_string(_data.at(1)) << "."
        << std::to_string(_data.at(2)) << "."
        << std::to_string(_data.at(3));
    return ss.str();
}

int IPv4SubnetMask::to_prefix() const{
    int prefix = 0;
    for (auto octet : _data){
        uint8_t one = 128u;
        for (int i = 0; i < 8; ++i){
            if (octet & one){
                prefix++;
            }
            one >>= 1;
        }
    }
    return prefix;
}

std::vector<uint8_t> IPv4SubnetMask::to_network_data() const{
    std::vector<uint8_t> result;
    for (int i = 0; i < IP_V4_SUBNET_MASK_LENGTH; ++i){
        result.push_back(_data.at(i));
    }
    return result;
}
