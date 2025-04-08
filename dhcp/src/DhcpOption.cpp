#include "DhcpOption.hpp"
#include <stdexcept>
#include "endians.hpp"

IntConstraint::IntConstraint(IntConstraintType type, int64_t min, int64_t max, std::vector<uint64_t> allowed_values)
    :   type(type), min(min), max(max), allowed_values(allowed_values)
{

}

IntConstraint IntConstraint::no_constraint(){
    return IntConstraint(IntConstraintType::NONE, 0, 0);
}

DhcpOptionPayloadDescription::DhcpOptionPayloadDescription()
    :   type(DhcpOptionPayloadType::NONE), is_list(false), min_len_in_elements(0), int_constraint(IntConstraint::no_constraint())
{

}

DhcpOptionPayloadDescription::DhcpOptionPayloadDescription(DhcpOptionPayloadType type, bool is_list, int min_len_in_elements, IntConstraint int_constraint)
    :   type(type), is_list(is_list), min_len_in_elements(min_len_in_elements), int_constraint(int_constraint)
{

}

int64_t DhcpOptionPayloadDescription::get_one_element_payload_length_in_bytes() const{
    switch (type)
    {
    case DhcpOptionPayloadType::BYTE_ARRAY: return 1;
    case DhcpOptionPayloadType::FLAG: return 1;
    case DhcpOptionPayloadType::INT_32: return 4;
    case DhcpOptionPayloadType::IP_ADDRESS: return 4;
    case DhcpOptionPayloadType::IP_ADDRESS_WITH_SUBNET_MASK: return 8;
    case DhcpOptionPayloadType::TWO_IP_ADDRESSES: return 8;
    case DhcpOptionPayloadType::NONE: return 0;
    case DhcpOptionPayloadType::STRING: return 1;
    case DhcpOptionPayloadType::SUBNET_MASK: return 4;
    case DhcpOptionPayloadType::UINT_16: return 2;
    case DhcpOptionPayloadType::UINT_32: return 4;
    case DhcpOptionPayloadType::UINT_8: return 1;
    case DhcpOptionPayloadType::UINT_ENUM: return 1;
    case DhcpOptionPayloadType::VENDOR_SPECIFIC_FIELD: return VARIABLE_DHCP_OPTION_LENGTH;
    }
    throw std::runtime_error("Неизвестный тип поля DHCP свойства");
}


DhcpOptionPayloadDescription DhcpOptionPayloadDescription::no_payload(){
    return DhcpOptionPayloadDescription(DhcpOptionPayloadType::NONE, false, 0, IntConstraint::no_constraint());
}


bool DhcpOptionPayloadDescription::is_correct_uint(uint32_t value){
    if (!(type == DhcpOptionPayloadType::UINT_32 || type == DhcpOptionPayloadType::UINT_16
        || type == DhcpOptionPayloadType::UINT_8 || type == DhcpOptionPayloadType::UINT_ENUM)){
        throw std::runtime_error("несоответствие типа DhcpOptionPayloadType");
    }
    switch (int_constraint.type)
    {
        case IntConstraintType::NONE: return true;
        case IntConstraintType::FROM_MIN: return value >= int_constraint.min;
        case IntConstraintType::RANGE: return value >= int_constraint.min && value <= int_constraint.max;
        case IntConstraintType::UINT_ENUM:{
            bool match = false;
            for (auto allowed_value : int_constraint.allowed_values){
                if (allowed_value == value){
                    match = true;
                    break;
                }
            }
            return match;
        }
    }
    return false;
}

bool DhcpOptionPayloadDescription::is_correct_int(int32_t value){
    if (type != DhcpOptionPayloadType::INT_32){
        throw std::runtime_error("несоответствие типа DhcpOptionPayloadType");
    }
    switch (int_constraint.type)
    {
        case IntConstraintType::NONE: return true;
        case IntConstraintType::FROM_MIN: return value >= int_constraint.min;
        case IntConstraintType::RANGE: return value >= int_constraint.min && value <= int_constraint.max;
        case IntConstraintType::UINT_ENUM:
            for (auto allowed_value : int_constraint.allowed_values){
                if (allowed_value == (unsigned int)value) {
                    return true;
                }
            }
            return false;
    }
    return false;
}

DhcpOptionDescription::DhcpOptionDescription(int code, int64_t payload_length, DhcpOptionPayloadDescription payload_description)
    :   code(code), payload_length(payload_length), payload_description(payload_description)
{

}

std::map<int, DhcpOptionDescription> options_descriptions = {
    {0, DhcpOptionDescription{0, 0, DhcpOptionPayloadDescription::no_payload()}},
    {255, DhcpOptionDescription{255, 0, DhcpOptionPayloadDescription::no_payload()}},
    {1, DhcpOptionDescription{1, 4, DhcpOptionPayloadDescription{DhcpOptionPayloadType::SUBNET_MASK, false, 0, IntConstraint::no_constraint()}}},
    {2, DhcpOptionDescription{2, 4, DhcpOptionPayloadDescription{DhcpOptionPayloadType::INT_32, false, 0, IntConstraint::no_constraint()}}},
    {3, DhcpOptionDescription{3, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {4, DhcpOptionDescription{4, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {5, DhcpOptionDescription{5, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {6, DhcpOptionDescription{6, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {7, DhcpOptionDescription{7, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {8, DhcpOptionDescription{8, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {9, DhcpOptionDescription{9, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {10, DhcpOptionDescription{10, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {11, DhcpOptionDescription{11, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {12, DhcpOptionDescription{12, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::STRING, false, 1, IntConstraint::no_constraint()}}},
    {13, DhcpOptionDescription{13, 2, DhcpOptionPayloadDescription{DhcpOptionPayloadType::UINT_16, false, 0, IntConstraint::no_constraint()}}},
    {14, DhcpOptionDescription{14, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::STRING, false, 1, IntConstraint::no_constraint()}}},
    //ToDo добавить в конструктор PayloadDescription функцию валидатор для более сложных валидаций как у 12 опции
    {15, DhcpOptionDescription{15, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::STRING, false, 1, IntConstraint::no_constraint()}}},
    {16, DhcpOptionDescription{16, 4, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, false, 0, IntConstraint::no_constraint()}}},
    {17, DhcpOptionDescription{17, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::STRING, false, 1, IntConstraint::no_constraint()}}},
    {18, DhcpOptionDescription{18, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::STRING, false, 1, IntConstraint::no_constraint()}}},
    {19, DhcpOptionDescription{19, 1, DhcpOptionPayloadDescription{DhcpOptionPayloadType::FLAG, false, 0, IntConstraint::no_constraint()}}},
    {20, DhcpOptionDescription{20, 1, DhcpOptionPayloadDescription{DhcpOptionPayloadType::FLAG, false, 0, IntConstraint::no_constraint()}}},
    {21, DhcpOptionDescription{21, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS_WITH_SUBNET_MASK, true, 1, IntConstraint::no_constraint()}}},
    {22, DhcpOptionDescription{22, 2, DhcpOptionPayloadDescription{DhcpOptionPayloadType::UINT_16, false, 0, IntConstraint{IntConstraintType::FROM_MIN, 576, 0, {}}}}},
    {23, DhcpOptionDescription{23, 1, DhcpOptionPayloadDescription{DhcpOptionPayloadType::UINT_8, false, 0, IntConstraint{IntConstraintType::RANGE, 1, 255, {}}}}},
    {24, DhcpOptionDescription{24, 4, DhcpOptionPayloadDescription{DhcpOptionPayloadType::UINT_32, false, 0, IntConstraint::no_constraint()}}},
    {25, DhcpOptionDescription{25, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::UINT_16, true, 2, IntConstraint::no_constraint()}}},
    {26, DhcpOptionDescription{26, 2, DhcpOptionPayloadDescription{DhcpOptionPayloadType::UINT_16, false, 0, IntConstraint{IntConstraintType::FROM_MIN, 68, 0, {}}}}},
    {27, DhcpOptionDescription{27, 1, DhcpOptionPayloadDescription{DhcpOptionPayloadType::FLAG, false, 0, IntConstraint::no_constraint()}}},
    {28, DhcpOptionDescription{28, 4, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, false, 0, IntConstraint::no_constraint()}}},
    {29, DhcpOptionDescription{29, 1, DhcpOptionPayloadDescription{DhcpOptionPayloadType::FLAG, false, 0, IntConstraint::no_constraint()}}},
    {30, DhcpOptionDescription{30, 1, DhcpOptionPayloadDescription{DhcpOptionPayloadType::FLAG, false, 0, IntConstraint::no_constraint()}}},
    {31, DhcpOptionDescription{31, 1, DhcpOptionPayloadDescription{DhcpOptionPayloadType::FLAG, false, 0, IntConstraint::no_constraint()}}},
    {32, DhcpOptionDescription{32, 4, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, false, 0, IntConstraint::no_constraint()}}},
    {33, DhcpOptionDescription{33, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::TWO_IP_ADDRESSES, true, 1, IntConstraint::no_constraint()}}},
    {34, DhcpOptionDescription{34, 1, DhcpOptionPayloadDescription{DhcpOptionPayloadType::FLAG, false, 0, IntConstraint::no_constraint()}}},
    {35, DhcpOptionDescription{35, 4, DhcpOptionPayloadDescription{DhcpOptionPayloadType::UINT_32, false, 0, IntConstraint::no_constraint()}}},
    {36, DhcpOptionDescription{36, 1, DhcpOptionPayloadDescription{DhcpOptionPayloadType::FLAG, false, 0, IntConstraint::no_constraint()}}},
    {37, DhcpOptionDescription{37, 1, DhcpOptionPayloadDescription{DhcpOptionPayloadType::UINT_8, false, 0, IntConstraint{IntConstraintType::FROM_MIN, 1, 0, {}}}}},
    {38, DhcpOptionDescription{38, 4, DhcpOptionPayloadDescription{DhcpOptionPayloadType::UINT_32, false, 0, IntConstraint::no_constraint()}}},
    {39, DhcpOptionDescription{39, 1, DhcpOptionPayloadDescription{DhcpOptionPayloadType::FLAG, false, 0, IntConstraint::no_constraint()}}},
    {40, DhcpOptionDescription{40, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::STRING, false, 1, IntConstraint::no_constraint()}}},
    {41, DhcpOptionDescription{41, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {42, DhcpOptionDescription{42, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {43, DhcpOptionDescription{43, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::VENDOR_SPECIFIC_FIELD, false, 1, IntConstraint::no_constraint()}}},
    {44, DhcpOptionDescription{44, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {45, DhcpOptionDescription{45, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {46, DhcpOptionDescription{46, 1, DhcpOptionPayloadDescription{DhcpOptionPayloadType::UINT_ENUM, false, 0, IntConstraint{IntConstraintType::UINT_ENUM, 0, 0, {0x1, 0x2, 0x4, 0x8}}}}},
    {47, DhcpOptionDescription{47, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::STRING, false, 1, IntConstraint::no_constraint()}}},
    {48, DhcpOptionDescription{48, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {49, DhcpOptionDescription{49, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {64, DhcpOptionDescription{64, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::STRING, false, 1, IntConstraint::no_constraint()}}},
    {65, DhcpOptionDescription{65, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {68, DhcpOptionDescription{68, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 0, IntConstraint::no_constraint()}}},
    {69, DhcpOptionDescription{69, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {70, DhcpOptionDescription{70, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {71, DhcpOptionDescription{71, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {72, DhcpOptionDescription{72, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {73, DhcpOptionDescription{73, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {74, DhcpOptionDescription{74, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {75, DhcpOptionDescription{75, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {76, DhcpOptionDescription{76, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {50, DhcpOptionDescription{50, 4, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, false, 0, IntConstraint::no_constraint()}}},
    {51, DhcpOptionDescription{51, 4, DhcpOptionPayloadDescription{DhcpOptionPayloadType::UINT_32, false, 0, IntConstraint::no_constraint()}}},
    {52, DhcpOptionDescription{52, 1, DhcpOptionPayloadDescription{DhcpOptionPayloadType::UINT_ENUM, false, 0, IntConstraint{IntConstraintType::UINT_ENUM, 0, 0, {1, 2, 3}}}}},
    {66, DhcpOptionDescription{66, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::STRING, false, 1, IntConstraint::no_constraint()}}},
    {67, DhcpOptionDescription{67, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::STRING, false, 1, IntConstraint::no_constraint()}}},
    {53, DhcpOptionDescription{53, 1, DhcpOptionPayloadDescription{DhcpOptionPayloadType::UINT_ENUM, false, 0, IntConstraint{IntConstraintType::UINT_ENUM, 0, 0, {1, 2, 3, 4, 5, 6, 7, 8}}}}},
    {54, DhcpOptionDescription{54, 4, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, false, 0, IntConstraint::no_constraint()}}},
    {55, DhcpOptionDescription{55, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::BYTE_ARRAY, false, 1, IntConstraint::no_constraint()}}},
    {56, DhcpOptionDescription{56, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::STRING, false, 1, IntConstraint::no_constraint()}}},
    {57, DhcpOptionDescription{57, 2, DhcpOptionPayloadDescription{DhcpOptionPayloadType::UINT_16, false, 0, IntConstraint{IntConstraintType::FROM_MIN, 576, 0, {}}}}},
    {58, DhcpOptionDescription{58, 4, DhcpOptionPayloadDescription{DhcpOptionPayloadType::UINT_32, false, 0, IntConstraint::no_constraint()}}},
    {59, DhcpOptionDescription{59, 4, DhcpOptionPayloadDescription{DhcpOptionPayloadType::UINT_32, false, 0, IntConstraint::no_constraint()}}},
    {60, DhcpOptionDescription{60, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::STRING, false, 1, IntConstraint::no_constraint()}}},
    {61, DhcpOptionDescription{61, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::BYTE_ARRAY, false, 2, IntConstraint::no_constraint()}}},
};


// итераторы на полезную нагрузку опции, не захватывая первые два байта кода и длины опции
DhcpOption::DhcpOption(int code, int64_t real_payload_length, std::vector<uint8_t>::iterator payload_begin, std::vector<uint8_t>::iterator payload_end){
    description = options_descriptions.at(code);
    if (description.payload_length == VARIABLE_DHCP_OPTION_LENGTH){
        this->real_payload_length = real_payload_length;
    } else {
        this->real_payload_length = description.payload_length;
        if (description.payload_length != real_payload_length){
            throw std::runtime_error("несовпадение реальной длины свойства и длины в стандарте");
        }
    }
    if (description.payload_description.type == DhcpOptionPayloadType::NONE){
        throw std::runtime_error("Нельзя задать пустое значение");
    }

    auto iter = payload_begin;
    auto element_size_in_bytes = description.payload_description.get_one_element_payload_length_in_bytes();
    while (iter != payload_end){
        switch (description.payload_description.type)
        {
            case DhcpOptionPayloadType::IP_ADDRESS: process_ip_address(iter, iter + element_size_in_bytes); break;
            case DhcpOptionPayloadType::SUBNET_MASK: process_subnet_mask(iter, iter + element_size_in_bytes); break;
            case DhcpOptionPayloadType::IP_ADDRESS_WITH_SUBNET_MASK: process_ip_address_with_mask(iter, iter + element_size_in_bytes); break;
            case DhcpOptionPayloadType::TWO_IP_ADDRESSES: process_two_ip_addresses(iter, iter + element_size_in_bytes); break;
            case DhcpOptionPayloadType::UINT_8: process_uint8(iter); break;
            case DhcpOptionPayloadType::UINT_16: process_uint16(iter, iter + element_size_in_bytes); break;
            case DhcpOptionPayloadType::UINT_32: process_uint32(iter, iter + element_size_in_bytes); break;
            case DhcpOptionPayloadType::INT_32: process_int32(iter, iter + element_size_in_bytes); break;
            case DhcpOptionPayloadType::FLAG: process_flag(iter); break;
            case DhcpOptionPayloadType::UINT_ENUM: process_uint_enum(iter); break;

            case DhcpOptionPayloadType::VENDOR_SPECIFIC_FIELD:
                process_vendor_specific_field(iter, payload_end);
                return;
            case DhcpOptionPayloadType::BYTE_ARRAY:
                process_byte_array(iter, payload_end);
                return;
            case DhcpOptionPayloadType::STRING:
                process_string(iter, payload_end);
                return;

            default: throw std::runtime_error("данный тип полезной нагрузки опции не может использоваться в списке");
        }
        iter += element_size_in_bytes;
    }
}


void DhcpOption::process_vendor_specific_field(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end){
    std::vector<uint8_t>value(begin, end);
    real_values.push_back(value);
}

void DhcpOption::process_byte_array(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end){
    std::vector<uint8_t> byte_array{begin, end};
    if (description.payload_length == VARIABLE_DHCP_OPTION_LENGTH
        && byte_array.size() < (unsigned int)description.payload_description.min_len_in_elements)
    {
        throw std::runtime_error("длина опции меньше чем минимальная");
    }
    if (description.payload_length != (int64_t)byte_array.size()){
        throw std::runtime_error{"несовпадение длины"};
    }
    real_values.push_back(byte_array);
}

void DhcpOption::process_string(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end){
    std::basic_string<uint8_t>value(begin, end);
    real_values.push_back(value);
}

void DhcpOption::process_ip_address(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end){
    IPv4Address value = IPv4Address::from_big_endian_bytes(begin, end);
    real_values.push_back(value);
}

void DhcpOption::process_subnet_mask(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end){
    IPv4SubnetMask subnet_mask = IPv4SubnetMask::from_big_endian_bytes(begin, end);
    real_values.push_back(subnet_mask);
}

void DhcpOption::process_ip_address_with_mask(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end){
    IPv4Address ip_address = IPv4Address::from_big_endian_bytes(begin, begin + 4);
    IPv4SubnetMask subnet_mask = IPv4SubnetMask::from_big_endian_bytes(begin + 4, end);
    real_values.push_back(std::pair{ip_address, subnet_mask});
}

void DhcpOption::process_two_ip_addresses(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end){
    IPv4Address ip1 = IPv4Address::from_big_endian_bytes(begin, begin + 4);
    IPv4Address ip2 = IPv4Address::from_big_endian_bytes(begin + 4, end);
    real_values.push_back(std::pair(ip1, ip2));
}

void DhcpOption::process_uint8(std::vector<uint8_t>::iterator iter){
    uint8_t value = *iter;
    if (!description.payload_description.is_correct_uint(value)){
        throw std::runtime_error("Значение опции не соответствует ограничениям");
    }
    real_values.push_back(value);
}

void DhcpOption::process_uint16(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end){
    uint16_t value = network_to_host_endian<uint16_t>(begin, end);
    if (!description.payload_description.is_correct_uint(value)){
        throw std::runtime_error("Значение опции не соответствует ограничениям");
    }
    real_values.push_back(value);
}

void DhcpOption::process_uint32(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end){
    uint32_t value = network_to_host_endian<uint32_t>(begin, end);
    if (!description.payload_description.is_correct_uint(value)){
        throw std::runtime_error("Значение опции не соответствует ограничениям");
    }
    real_values.push_back(value);
}

void DhcpOption::process_int32(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end){
    int32_t value = network_to_host_endian<int32_t>(begin, end);
    if (!description.payload_description.is_correct_int(value)){
        throw std::runtime_error("Значение опции не соответствует ограничениям");
    }
    real_values.push_back(value);
}

void DhcpOption::process_flag(std::vector<uint8_t>::iterator iter){
    bool value = *iter;
    real_values.push_back(value);
}

void DhcpOption::process_uint_enum(std::vector<uint8_t>::iterator iter){
    uint8_t value = *iter;
    if (!description.payload_description.is_correct_uint(value)){
        throw std::runtime_error("Значение опции не соответствует ограничениям");
    }
    real_values.push_back(value);
}
