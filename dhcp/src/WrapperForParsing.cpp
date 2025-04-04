#include "WrapperForParsing.hpp"


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
