#pragma once

#include <string>

// выдает по одному символу
class WrapperForParsing{
public:
    static inline constexpr char EOT = 3;
    WrapperForParsing(const std::string& str);
    void get_next();
    void skip_whitespaces();
    bool is_end() const;
    void check(char c);
    char ch;
private:
    std::string _data;
    size_t pos;
};
