#pragma once

#include <string>

enum class Color{
    BLACK = 0,
    RED = 1,
    GREEN = 2,
    YELLOW = 3,
    BLUE = 4,
    PURPLE = 5,
    DARK_GREEN = 6,
    WHITE = 7
};

class ConsoleController{
public:
    ConsoleController(int in_descriptor, int out_descriptor);
    int getkey();
    void move_cursor_to_left(int number);
    void clear_line_from_cursor_position();
    void write_str( const std::string& str);
    void change_color(Color color);
private:
    int _in_descriptor;
    int _out_descriptor;
};
