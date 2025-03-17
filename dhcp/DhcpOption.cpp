#include "DhcpOption.h"
#include <stdexcept>


IntConstraint::IntConstraint(IntConstraintType type, int64_t min, int64_t max, std::vector<uint64_t> allowed_values)
    :   type(type), min(min), max(max), allowed_values(allowed_values)
{

}

IntConstraint IntConstraint::no_constraint(){
    return IntConstraint(IntConstraintType::NONE, 0, 0);
}

DhcpOptionField::DhcpOptionField(DhcpOptionFieldType type, bool is_list, int min_len, IntConstraint int_constraint)
    :   type(type), is_list(is_list), min_len(min_len), int_constraint(int_constraint)
{

}


void DhcpOptionField::set_real_value(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end){
    switch (type)
    {
        case DhcpOptionFieldType::BYTE_ARRAY:{
            std::vector<uint8_t> value;
            value.insert(value.begin(), begin, end);
            real_values.push_back(value);
            break;
        }
        case DhcpOptionFieldType::FLAG:{
            bool value = *begin;
            real_values.push_back(value);
            break;
        }
        case DhcpOptionFieldType::INT_32:{
            int32_t value;
            // ToDo
            real_values.push_back(value);
            break;
        }
        case DhcpOptionFieldType::IP_ADDRESS:{
            IpAddress value(begin, end);
            real_values.push_back(value);
            break;
        }
        case DhcpOptionFieldType::NONE:
            throw std::runtime_error("Нельзя задать пустое значение");
        case DhcpOptionFieldType::STRING:{
            std::basic_string<uint8_t>value(begin, end);
            real_values.push_back(value);
            break;
        }
        case DhcpOptionFieldType::SUBNET_MASK:{
            std::basic_string<uint8_t>value(begin, end);
            //ToDo
            real_values.push_back(value);
            break;
        }
        case DhcpOptionFieldType::UINT_16:{
            std::basic_string<uint8_t>value(begin, end);
            //ToDo
            real_values.push_back(value);
            break;
        }
        case DhcpOptionFieldType::UINT_32:{
            std::basic_string<uint8_t>value(begin, end);
            //ToDo
            real_values.push_back(value);
            break;
        }
        case DhcpOptionFieldType::UINT_8:{
            std::basic_string<uint8_t>value(begin, end);
            //ToDo
            real_values.push_back(value);
            break;
        }
        case DhcpOptionFieldType::UINT_ENUM:{
            std::basic_string<uint8_t>value(begin, end);
            //ToDo
            real_values.push_back(value);
            break;
        }
        case DhcpOptionFieldType::VENDOR_SPECIFIC_FIELD:{
            std::basic_string<uint8_t>value(begin, end);
            //ToDo
            real_values.push_back(value);
            break;
        }
    }
    throw std::runtime_error("Неизвестный тип поля DHCP свойства");
}

int64_t DhcpOptionField::get_one_element_payload_length_in_bytes() const{
    switch (type)
    {
    case DhcpOptionFieldType::BYTE_ARRAY: return 1;
    case DhcpOptionFieldType::FLAG: return 1;
    case DhcpOptionFieldType::INT_32: return 4;
    case DhcpOptionFieldType::IP_ADDRESS: return 4;
    case DhcpOptionFieldType::NONE: return 0;
    case DhcpOptionFieldType::STRING: return 1;
    case DhcpOptionFieldType::SUBNET_MASK: return 4;
    case DhcpOptionFieldType::UINT_16: return 2;
    case DhcpOptionFieldType::UINT_32: return 4;
    case DhcpOptionFieldType::UINT_8: return 1;
    case DhcpOptionFieldType::UINT_ENUM: return 1;
    case DhcpOptionFieldType::VENDOR_SPECIFIC_FIELD: VARIABLE_DHCP_OPTION_LENGTH;
    }
    throw std::runtime_error("Неизвестный тип поля DHCP свойства");
}

DhcpOption::DhcpOption(int code, int64_t length, std::vector<DhcpOptionField> fields)
    :   code(code), length(length), fields(fields)
{

}

std::vector<DhcpOption> options = {
    DhcpOption{0, 0, {}},
    DhcpOption{255, 0, {}},
    DhcpOption{1, 4, {
        DhcpOptionField{DhcpOptionFieldType::SUBNET_MASK, false, 0, IntConstraint::no_constraint()}
    }},
    DhcpOption{2, 4, {
        DhcpOptionField{DhcpOptionFieldType::INT_32, false, 0, IntConstraint::no_constraint()}
    }},
    DhcpOption{3, VARIABLE_DHCP_OPTION_LENGTH, {
        DhcpOptionField{DhcpOptionFieldType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}
    }},
    DhcpOption{4, VARIABLE_DHCP_OPTION_LENGTH, {
        DhcpOptionField{DhcpOptionFieldType::IP_ADDRESS, true, 1, IntConstraint::no_constraint()}
    }}
};
