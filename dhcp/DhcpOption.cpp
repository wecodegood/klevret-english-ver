#include "DhcpOption.h"


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
