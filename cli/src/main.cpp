#include <iostream>
#include "Command.hpp"
#include "ConsoleController.hpp"

void print_tree(const CommandTree& root, int tab=0){
    for (const auto& node : root.childs){
        for (int i = 0; i < tab; ++i){
            std::cout << "--";
        }
        std::cout << to_string(node.command_element)  << "\n";
        print_tree(node, tab + 1);
    }
}

void print_title(Console &console){
    Color old_color = console.current_text_color;
    console.change_text_color(Color::GREEN);
    console.write_str("klevret> ");
    console.change_text_color(old_color);
}

int main(){
    CommandTree root = create_command_tree();
    CommandTree *current_node = &root;
    print_tree(root);
    Console console(fileno(stdin), fileno(stdout));
    auto commands = get_all_commands();
    char prev = 0;
    print_title(console);
    while (true){
        char c;
        c = console.getkey();
        if (c != -1){
            //console.write_str("<" + std::to_string((int)c) + ">");

            switch (c)
            {
                case Console::TAB:
                    console.move_cursor_to_left(1);
                    console.clear_line_from_cursor_position();
                    console.write_str("\n");
                    for (auto& node : current_node->childs){
                        console.write_str(to_string(node.command_element) +"\n");
                    }
                    console.write_str("\r");
                    print_title(console);
                    if (prev == Console::ENTER)
                        for (int k = 0; k < 9; ++k){
                            console.current_command_input += (std::string("") + (char)27 + (char)91 + (char)67);
                        }
                    console.write_str(console.current_command_input);
                    break;
                case Console::ENTER:
                    console.move_cursor_to_left(2);
                    console.clear_line_from_cursor_position();
                    console.write_str("\n");
                    //ToDo handle input
                    console.current_command_input = "";
                    console.current_command_input_cursor_pos = 0;
                    print_title(console);
                default:
                    console.current_command_input += c;
                    break;
            }
            prev = c;
        }
    }
    std::cout << "it is CLI!\n";
}
