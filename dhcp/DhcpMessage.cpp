#include "DhcpMessage.h"


DhcpMessage::DhcpMessage()
    :   op(MessageType::BOOTREQUEST),
        htype(HardwareAddressType::Ethernet_10Mb),
        hlen(get_hardware_address_length(htype))

{

}

DhcpMessage::DhcpMessage(std::vector<uint8_t> data){

}

std::vector<uint8_t> DhcpMessage::to_network_data(){

}
