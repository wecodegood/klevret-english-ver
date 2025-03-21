#include "DhcpOption.h"
#include <stdexcept>
#include "endians.h"

IntConstraint::IntConstraint(IntConstraintType type, int64_t min, int64_t max, std::vector<uint64_t> allowed_values)
    :   type(type), min(min), max(max), allowed_values(allowed_values)
{

}

IntConstraint IntConstraint::no_constraint(){
    return IntConstraint(IntConstraintType::NONE, 0, 0);
}

DhcpOptionPayloadDescription::DhcpOptionPayloadDescription()
    :   type(DhcpOptionPayloadType::NONE), is_list(false), min_len(0), int_constraint(IntConstraint::no_constraint())
{

}

DhcpOptionPayloadDescription::DhcpOptionPayloadDescription(DhcpOptionPayloadType type, bool is_list, int min_len, IntConstraint int_constraint)
    :   type(type), is_list(is_list), min_len(min_len), int_constraint(int_constraint)
{

}

int64_t DhcpOptionPayloadDescription::get_one_element_payload_length_in_bytes() const{
    switch (type)
    {
    case DhcpOptionPayloadType::BYTE_ARRAY: return 1;
    case DhcpOptionPayloadType::FLAG: return 1;
    case DhcpOptionPayloadType::INT_32: return 4;
    case DhcpOptionPayloadType::IP_ADDRESS: return 4;
    case DhcpOptionPayloadType::NONE: return 0;
    case DhcpOptionPayloadType::STRING: return 1;
    case DhcpOptionPayloadType::SUBNET_MASK: return 4;
    case DhcpOptionPayloadType::UINT_16: return 2;
    case DhcpOptionPayloadType::UINT_32: return 4;
    case DhcpOptionPayloadType::UINT_8: return 1;
    case DhcpOptionPayloadType::UINT_ENUM: return 1;
    case DhcpOptionPayloadType::VENDOR_SPECIFIC_FIELD: VARIABLE_DHCP_OPTION_LENGTH;
    }
    throw std::runtime_error("Неизвестный тип поля DHCP свойства");
}


DhcpOptionPayloadDescription DhcpOptionPayloadDescription::no_payload(){
    return DhcpOptionPayloadDescription(DhcpOptionPayloadType::NONE, false, 0, IntConstraint::no_constraint());
}


bool DhcpOptionPayloadDescription::is_correct_uint(uint32_t value){
    if (!(type == DhcpOptionPayloadType::UINT_32 || type == DhcpOptionPayloadType::UINT_16 || type == DhcpOptionPayloadType::UINT_8)){
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

DhcpOptionDescription::DhcpOptionDescription(int code, int64_t length, DhcpOptionPayloadDescription payload_description)
    :   code(code), length(length), payload_description(payload_description)
{

}

std::map<int, DhcpOptionDescription> options_descriptions = {
    {0, DhcpOptionDescription{0, 0, DhcpOptionPayloadDescription::no_payload()}},
    {255, DhcpOptionDescription{255, 0, DhcpOptionPayloadDescription::no_payload()}},
    {1, DhcpOptionDescription{1, 4, DhcpOptionPayloadDescription{DhcpOptionPayloadType::SUBNET_MASK, false, 0, IntConstraint::no_constraint()}}},
    {2, DhcpOptionDescription{2, 4, DhcpOptionPayloadDescription{DhcpOptionPayloadType::INT_32, false, 0, IntConstraint::no_constraint()}}},
    {3, DhcpOptionDescription{3, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}},
    {4, DhcpOptionDescription{4, VARIABLE_DHCP_OPTION_LENGTH, DhcpOptionPayloadDescription{DhcpOptionPayloadType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}}}
};


// итераторы на полезную нагрузку опции, не захватывая первые два байта кода и длины опции
DhcpOption::DhcpOption(int code, int64_t length, std::vector<uint8_t>::iterator payload_begin, std::vector<uint8_t>::iterator payload_end){
    description = options_descriptions.at(code);
    if (description.length == VARIABLE_DHCP_OPTION_LENGTH){
        real_length = length;
    } else {
        real_length = description.length;
        if (description.length != length){
            throw std::runtime_error("несовпадение реальной длины свойства и длины в стандарте");
        }
    }
    switch (description.payload_description.type)
    {
        case DhcpOptionPayloadType::BYTE_ARRAY:{
            std::vector<uint8_t> byte_array;
            byte_array.insert(byte_array.begin(), payload_begin, payload_end);
            if (description.length == VARIABLE_DHCP_OPTION_LENGTH
                && byte_array.size() < description.payload_description.min_len)
            {
                throw std::runtime_error("длина опции меньше чем минимальная");
            } else {
                if (description.length != byte_array.size()){
                    throw std::runtime_error{"несовпадение длины"};
                }
            }
            real_values.push_back(byte_array);
            return;
        }
        case DhcpOptionPayloadType::FLAG:{
            bool value = *payload_begin;
            real_values.push_back(value);
            return;
        }
        case DhcpOptionPayloadType::INT_32:{
            int32_t value = int32_big_endian_to_host_endian(payload_begin, payload_end);
            if (!description.payload_description.is_correct_int(value)){
                throw std::runtime_error("Значение опции не соответствует ограничениям");
            }
            real_values.push_back(value);
            return;
        }
        case DhcpOptionPayloadType::IP_ADDRESS:{
            IpAddress value(payload_begin, payload_end);
            real_values.push_back(value);
            return;
        }
        case DhcpOptionPayloadType::NONE:
            throw std::runtime_error("Нельзя задать пустое значение");
        case DhcpOptionPayloadType::STRING:{
            std::basic_string<uint8_t>value(payload_begin, payload_end);
            real_values.push_back(value);
            return;
        }
        case DhcpOptionPayloadType::SUBNET_MASK:{
            subnet_mask_t subnet_mask = uint32_big_endian_to_host_endian(payload_begin, payload_end);
            real_values.push_back(subnet_mask);
            return;
        }
        case DhcpOptionPayloadType::UINT_16:{
            uint16_t value = uint16_big_endian_to_host_endian(payload_begin, payload_end);
            if (!description.payload_description.is_correct_uint(value)){
                throw std::runtime_error("Значение опции не соответствует ограничениям");
            }
            real_values.push_back(value);
            return;
        }
        case DhcpOptionPayloadType::UINT_32:{
            uint32_t value = uint32_big_endian_to_host_endian(payload_begin, payload_end);
            if (!description.payload_description.is_correct_uint(value)){
                throw std::runtime_error("Значение опции не соответствует ограничениям");
            }
            real_values.push_back(value);
            return;
        }
        case DhcpOptionPayloadType::UINT_8:{
            uint8_t value = *payload_begin;
            if (!description.payload_description.is_correct_uint(value)){
                throw std::runtime_error("Значение опции не соответствует ограничениям");
            }
            real_values.push_back(value);
            return;
        }
        case DhcpOptionPayloadType::UINT_ENUM:{
            uint8_t value = *payload_begin;
            if (!description.payload_description.is_correct_uint(value)){
                throw std::runtime_error("Значение опции не соответствует ограничениям");
            }
            real_values.push_back(value);
            return;
        }
        case DhcpOptionPayloadType::VENDOR_SPECIFIC_FIELD:{
            std::vector<uint8_t>value(payload_begin, payload_end);
            real_values.push_back(value);
            return;
        }
    }
    throw std::runtime_error("Неизвестный тип поля DHCP свойства");
}
