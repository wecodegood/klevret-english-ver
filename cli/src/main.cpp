#include <iostream>
#include "Command.hpp"
#include "Console.hpp"


void print_tree(const CommandTree& root, int tab=0){
    for (const auto& node : root.childs){
        for (int i = 0; i < tab; ++i){
            std::cout << "--";
        }
        std::cout << to_string(node.command_element)  << "\n";
        print_tree(node, tab + 1);
    }
}

void print_title(){
    Color old_color = Console::Instance().current_text_color;
    Console::Instance().change_text_color(Color::GREEN);
    Console::Instance().write_str("klevret> ");
    Console::Instance().change_text_color(old_color);
}

int main(){
    auto commands = get_all_commands();
    CommandTree command_tree_root;
    CommandTree *current_command_tree_node;
    char prev = 0;
    Console& console = Console::Instance();

    print_tree(command_tree_root);
    print_title();

    while (true){
        char c = -1;
        c = Console::Instance().getkey();
        if (c != -1){
            switch (c){
                case Console::TAB:
                    console.move_cursor_to_left(1);
                    console.clear_line_from_cursor_position();
                    console.write_str("\n");
                    for (auto& node : current_command_tree_node->childs){
                        console.write_str(to_string(node.command_element) +"\n");
                    }
                    console.write_str("\r");
                    print_title();
                    if (prev == Console::ENTER){
                        for (int k = 0; k < 9; ++k){
                            console.current_command_input += (std::string("") + (char)27 + (char)91 + (char)67);
                        }
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
                    print_title();
                    break;
                default:
                    console.current_command_input += c;
                    break;
            }
        }
    }
}
