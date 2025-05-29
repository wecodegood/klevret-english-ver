#include "WrapperForParsing.hpp"

#include <stdexcept>


WrapperForParsing::WrapperForParsing(const std::string& str)
    :   _data(str), pos(0)
{
    get_next();
}

void WrapperForParsing::get_next(){
    if (pos == _data.size()){
        ch = EOT;
        return;
    }
    ch = _data[pos];
    pos++;
}

void WrapperForParsing::skip_whitespaces(){
    while (std::isspace(ch)){
        get_next();
    }
}


bool WrapperForParsing::is_end() const{
    return ch == EOT;
}


void WrapperForParsing::check(char c){
    using namespace std::string_literals;
    if (this->ch == c){
        get_next();
    } else {
        throw std::runtime_error("Ожидалось <"s + c + ">, но <"s + ch + ">"s);
    }
}
