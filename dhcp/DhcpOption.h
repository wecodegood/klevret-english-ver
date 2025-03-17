#pragma once

#include <cstdint>
#include <vector>
#include <variant>
#include <string>
#include "IpAddress.h"

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

enum class DhcpOptionFieldType{
    NONE,
    VENDOR_SPECIFIC_FIELD,
    BYTE_ARRAY,
    STRING,
    IP_ADDRESS,
    SUBNET_MASK,
    UINT_8,
    UINT_16,
    UINT_32,
    INT_32,
    FLAG,
    UINT_ENUM
};

using dhcp_option_field_real_value_t = std::variant<
    int64_t,
    uint64_t,
    std::vector<uint8_t>,
    std::basic_string<uint8_t>,
    IpAddress,
    subnet_mask_t,
    bool
>;

struct DhcpOptionField{
    DhcpOptionField(DhcpOptionFieldType type, bool is_list, int min_len, IntConstraint int_constraint);
    void set_real_value(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end);
    int64_t get_one_element_payload_length_in_bytes() const;

    DhcpOptionFieldType type = DhcpOptionFieldType::NONE;
    bool is_list = false;
    int min_len = 0; // for arrays and lists of fields
    IntConstraint int_constraint; // for int & uint & uint_enum
    std::vector<dhcp_option_field_real_value_t> real_values;
};

constexpr int VARIABLE_DHCP_OPTION_LENGTH = -1;

struct DhcpOption{
    DhcpOption(int code, int64_t length, std::vector<DhcpOptionField> fields);
    int code = 0;
    int64_t length = 0;
    std::vector<DhcpOptionField> fields;
};

// данные опции являются и описанием DHCP опций из стандарта RFC 2132 и могут содержать реальное значение
extern std::vector<DhcpOption> options;
