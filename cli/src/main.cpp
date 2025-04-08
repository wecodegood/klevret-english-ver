#include <iostream>
#include "Command.hpp"
#include "ConsoleController.hpp"


int main(){
    ConsoleController console(fileno(stdin), fileno(stdout));
    auto commands = get_all_commands();
    while (true){
        char c;
        c = console.getkey();
        if (c != -1){
            if (c == 127){
                console.move_cursor_to_left(3);
                console.clear_line_from_cursor_position();
            }
            for (auto & command : commands){
                if (command._elements[0].name_value[0] == c){
                    console.change_color(Color::DARK_GREEN);
                    console.write_str(command._elements[0].name_value.substr(1));
                    console.change_color(Color::WHITE);
                    console.move_cursor_to_left(command._elements[0].name_value.length() - 1);
                }
            }
        }
    }
    std::cout << "it is CLI!\n";
}
