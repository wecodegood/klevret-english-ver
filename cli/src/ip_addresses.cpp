#include "ip_addresses.hpp"

#include "WrapperForParsing.hpp"
#include <stdexcept>

IPv4Address::IPv4Address() : _data{0}{

}

int parse_octet(WrapperForParsing& wrap){
    int octet = 0;
    while(std::isdigit(wrap.ch)){
        octet = octet * 10 + (wrap.ch - '0');
        wrap.get_next();
    }
    if (octet < 0 || octet > 255){
        throw std::runtime_error("Октет должен быть в диапазоне [0..255]");
    }
    return octet;
}

IPv4Address::IPv4Address(const std::string& ip){
    WrapperForParsing wrap(ip);
    for (int i = 0; i < 4; ++i){
        _data[i] = parse_octet(wrap);
        if (i != 3){
            wrap.check('.');
        }
    }
}

IPv6Address::IPv6Address() : _data{0}{

}

IPv6Address::IPv6Address(const std::string& ip) : IPv6Address(){
    //ToDo
}

IPv4SubnetMask::IPv4SubnetMask() : _data{0}{

}

IPv4SubnetMask::IPv4SubnetMask(const std::string& mask) : IPv4SubnetMask(){

}

IPv6SubnetMask::IPv6SubnetMask() : _data{0}{

}

IPv6SubnetMask::IPv6SubnetMask(const std::string& mask) : IPv6SubnetMask(){

}
