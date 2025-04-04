#pragma once

#include <string>

// выдает по одному символу
class WrapperForParsing{
public:
    static inline constexpr char EOT = 3;
    WrapperForParsing(const std::string& str);
    void get_next();
    char ch;
private:
    std::string _data;
    size_t pos;
};
