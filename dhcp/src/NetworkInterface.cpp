#include "NetworkInterface.hpp"

#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <stdexcept>
#include "endians.hpp"
#include <sys/ioctl.h>
#include <linux/if.h>
#include <unistd.h>
#include <cstring>

std::vector<NetworkInterface> get_all_interfaces(){
    // https://man7.org/linux/man-pages/man3/getifaddrs.3.html
    // https://man7.org/linux/man-pages/man3/sockaddr.3type.html
    std::vector<NetworkInterface> result;
    ifaddrs *p_iffaddrs{nullptr};
    if (getifaddrs(&p_iffaddrs) != 0){
        throw std::runtime_error("Не удалось получить сетевые интерфейсы Linux");
    }
    auto current_iface = p_iffaddrs;
    while (current_iface){
        NetworkInterface new_iface;
        new_iface.name = std::string(current_iface->ifa_name);
        if (current_iface->ifa_addr->sa_family == AF_INET){
            new_iface.type = L3AddressType::IPv4;
            new_iface.network_address = IPv4Address(network_to_host_endian<uint32_t>(((sockaddr_in *)(current_iface->ifa_addr))->sin_addr.s_addr));
            new_iface.subnet_mask = IPv4SubnetMask(network_to_host_endian<uint32_t>(((sockaddr_in *)current_iface->ifa_netmask)->sin_addr.s_addr));
            int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
            ifreq ifr;
            strcpy(ifr.ifr_ifrn.ifrn_name, new_iface.name.c_str());
            ioctl(fd, SIOCGIFHWADDR, &ifr);
            close(fd);
            std::vector<uint8_t> mac_data(std::begin(ifr.ifr_ifru.ifru_hwaddr.sa_data), std::begin(ifr.ifr_ifru.ifru_hwaddr.sa_data) + MAC_ADDRESS_LENGTH);
            new_iface.mac_address = MacAddress::from_big_endian_bytes(mac_data.begin(), mac_data.end());
            result.push_back(new_iface);
        } else if (current_iface->ifa_addr->sa_family == AF_INET6){

        }
        current_iface = current_iface->ifa_next;
    }
    freeifaddrs(p_iffaddrs);
    return result;
}
