#pragma once

#include <string>
#include <vector>
#include <mutex>

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

class Console{
public:
    static Console& Instance();

    static constexpr int BACKSPACE = 127;
    static constexpr int TAB = 9;
    static constexpr int ENTER = 13;
    static constexpr int ESC = 27;
    //static constexpr int ARROW_LEFT = 27 91 68;
    //static constexpr int ARROW_RIGHT = 27 91 67;
    //static constexpr int ARROW_UP = 27 91 65;
    //static constexpr int ARROW_DOWN = 27 91 66;
    static constexpr std::string CSI = "\e[";

    int getkey();
    void move_cursor_to_left(int number);
    void move_cursor_to_right(int number);
    void clear_line_from_cursor_position();
    void write_str( const std::string& str);
    void change_text_color(Color color);
    void print_hints(const std::vector<std::string>& hints);

    std::string current_command_input = "";
    int current_command_input_cursor_pos = 0;
    Color current_text_color = Color::WHITE;

private:
    Console();
    ~Console(){};
    Console(const Console&)=default;
    Console& operator=(const Console&)=default;
    int _in_descriptor;
    int _out_descriptor;
};
