#pragma once

#include <cstdint>
#include <vector>
#include <variant>
#include <string>
#include "IpAddress.hpp"
#include <map>

enum class IntConstraintType{
    NONE,
    RANGE,
    FROM_MIN,
    UINT_ENUM
};

struct IntConstraint{
    IntConstraint(IntConstraintType type, int64_t min, int64_t max, std::vector<uint64_t> allowed_values={});
    static IntConstraint no_constraint();

    IntConstraintType type = IntConstraintType::NONE;
    int64_t min = 0;
    int64_t max = 0;
    std::vector<uint64_t> allowed_values; // for enum
};

enum class DhcpOptionPayloadType{
    NONE,
    VENDOR_SPECIFIC_FIELD,
    BYTE_ARRAY,
    STRING,
    IP_ADDRESS,
    SUBNET_MASK,
    IP_ADDRESS_WITH_SUBNET_MASK,
    TWO_IP_ADDRESSES,
    UINT_8,
    UINT_16,
    UINT_32,
    INT_32,
    FLAG,
    UINT_ENUM
};

using dhcp_option_field_real_value_t = std::variant<
    int32_t,
    uint32_t,
    uint16_t,
    uint8_t,
    std::vector<uint8_t>,
    std::basic_string<uint8_t>,
    IPv4Address,
    IPv4SubnetMask,
    std::pair<IPv4Address, IPv4SubnetMask>,
    std::pair<IPv4Address, IPv4Address>,
    bool
>;

struct DhcpOptionPayloadDescription{
    DhcpOptionPayloadDescription();
    DhcpOptionPayloadDescription(DhcpOptionPayloadType type, bool is_list, int min_len_in_elements, IntConstraint int_constraint);
    static DhcpOptionPayloadDescription no_payload();
    int64_t get_one_element_payload_length_in_bytes() const;
    bool is_correct_uint(uint32_t value);
    bool is_correct_int(int32_t value);

    DhcpOptionPayloadType type = DhcpOptionPayloadType::NONE;
    bool is_list = false;
    int min_len_in_elements = 0; // for arrays and lists of fields
    IntConstraint int_constraint; // for int & uint & uint_enum
};

constexpr int VARIABLE_DHCP_OPTION_LENGTH = -1;

struct DhcpOptionDescription{
    DhcpOptionDescription()=default;
    DhcpOptionDescription(int code, int64_t payload_length, DhcpOptionPayloadDescription payload_description);
    int code = 0;
    int64_t payload_length = 0;
    DhcpOptionPayloadDescription payload_description;
};

extern std::map<int, DhcpOptionDescription> options_descriptions;

class DhcpOption{
public:
    DhcpOption()=default;
    DhcpOption(int code, int64_t real_payload_length, std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end);
    int64_t real_payload_length;
    DhcpOptionDescription description;
    std::vector<dhcp_option_field_real_value_t> real_values;
    std::vector<uint8_t> to_network_data() const;
private:
    void process_vendor_specific_field(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end);
    void process_byte_array(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end);
    void process_string(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end);
    void process_ip_address(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end);
    void process_subnet_mask(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end);
    void process_ip_address_with_mask(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end);
    void process_two_ip_addresses(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end);
    void process_uint8(std::vector<uint8_t>::iterator iter);
    void process_uint16(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end);
    void process_uint32(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end);
    void process_int32(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end);
    void process_flag(std::vector<uint8_t>::iterator iter);
    void process_uint_enum(std::vector<uint8_t>::iterator iter);

};
