#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <string>


// RFC 1700, page 163
enum class HardwareAddressType : uint8_t{
    Ethernet_10Mb = 1,
    Experimental_Ethernet_3Mb,
    Amateur_Radio_AX_25,
    Proteon_ProNET_Token_Ring,
    Chaos,
    IEEE_802_Networks,
    ARCNET,
    Hyperchannel,
    Lanstar,
    Autonet_Short_Address,
    LocalTalk,
    LocalNet_IBM_PCNet_or_SYTEK_LocalNET,
    Ultra_link,
    SMDS,
    Frame_Relay,
    Asynchronous_Transmission_Mode_ATM,
    HDLC,
    Fibre_Channel,
    Asynchronous_Transmission_Mode_ATM_19,
    Serial_Line,
    Asynchronous_Transmission_Mode_ATM_21
};

constexpr int get_hardware_address_length(HardwareAddressType htype){
    switch (htype)
    {
        case HardwareAddressType::Ethernet_10Mb: return 6;

    }
    return 0;
}

constexpr int DHCP_HARDWARE_ADDRESS_MAX_LENGTH = 16;
using chaddr_t = std::array<uint8_t, DHCP_HARDWARE_ADDRESS_MAX_LENGTH>;

class HardwareAddress{
public:
    virtual chaddr_t to_chaddr()=0;
};

constexpr int MAC_ADDRESS_LENGTH = get_hardware_address_length(HardwareAddressType::Ethernet_10Mb);

class MacAddress : public HardwareAddress{
public:
    MacAddress()=default;
    static MacAddress from_big_endian_bytes(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end);
    static MacAddress* make_ptr_from_big_endian_bytes(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end);
    std::string to_string() const;
    virtual chaddr_t to_chaddr() override;

    const static uint8_t address_length = MAC_ADDRESS_LENGTH;
    std::array<uint8_t, MAC_ADDRESS_LENGTH> data;
};
