#include <iostream>
#include "Command.hpp"
#include "Console.hpp"
#include <boost/algorithm/string.hpp>
#include <stack>
#include <tuple>


void print_tree(const CommandTree& root, int tab=0){
    for (const auto& node : root.childs){
        if (node.command_element.handler == nullptr){
            for (int i = 0; i < tab; ++i){
                std::cout << "--";
            }
            std::cout << to_string(node.command_element)  << "\n";
        }
        print_tree(node, tab + 1);
    }
}

void print_title(){
    Color old_color = Console::Instance().current_text_color;
    Console::Instance().change_text_color(Color::GREEN);
    Console::Instance().write_str("klevret> ");
    Console::Instance().change_text_color(old_color);
}

enum class ESC_sequense{
    ARROW_LEFT,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    UNKNOWN
};

ESC_sequense to_esq_sequence(char ch_1, char ch_2){
    if (ch_1 == 91 && ch_2 == 68) return ESC_sequense::ARROW_LEFT;
    if (ch_1 == 91 && ch_2 == 67) return ESC_sequense::ARROW_RIGHT;
    return ESC_sequense::UNKNOWN;
}

std::tuple<CommandTree*, std::stack<CommandElementRealValue>> analyze_command(const std::string& command, CommandTree& root){
    std::stack<CommandElementRealValue> stack;
    std::vector<std::string> parts;
    boost::algorithm::split(parts, command, boost::is_any_of(" "), boost::algorithm::token_compress_on);
    CommandTree* current = &root;
    for (const auto& part : parts){
        for (auto& child : current->childs){
            if (check_command_element(part, child.command_element)){
                current = &child;
                switch (child.command_element.type){
                    case CommandElementType::IP_V4_ADDRESS:
                        stack.push(IPv4Address(part));
                        break;
                    case CommandElementType::IP_V6_ADDRESS:
                        stack.push(IPv6Address(part));
                        break;
                    case CommandElementType::IP_V4_SUBNET_MASK:
                        stack.push(IPv4SubnetMask(part));
                        break;
                    case CommandElementType::IP_V6_SUBNET_MASK:
                        stack.push(IPv6SubnetMask(part));
                        break;
                    default:
                        stack.push(part);
                        break;
                }
                break;
            }
        }
    }
    return std::tuple(current, stack);
}

int main(){
    auto commands = get_all_commands();
    CommandTree command_tree_root = create_command_tree();
    CommandTree *current_command_tree_node = &command_tree_root;
    std::stack<CommandElementRealValue> current_command_elements_stack;
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
                    std::tie(current_command_tree_node, current_command_elements_stack) = analyze_command(console.current_command_input, command_tree_root);
                    if (current_command_tree_node){
                        for (auto& node : current_command_tree_node->childs){
                            console.write_str(to_string(node.command_element) +"\n");
                        }
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
                    std::tie(current_command_tree_node, current_command_elements_stack) = analyze_command(console.current_command_input, command_tree_root);
                    if (current_command_tree_node->childs.size() == 1 && current_command_tree_node->childs.at(0).command_element.handler != nullptr){
                        current_command_tree_node->childs.at(0).command_element.handler(current_command_elements_stack);
                    }
                    //
                    console.current_command_input = "";
                    console.current_command_input_cursor_pos = 0;
                    print_title();
                    break;
                case Console::BACKSPACE:
                    ///
                    console.move_cursor_to_left(console.current_command_input_cursor_pos + 2);
                    console.clear_line_from_cursor_position();
                    if (console.current_command_input_cursor_pos != 0){
                        console.current_command_input = console.current_command_input.erase(console.current_command_input_cursor_pos - 1, 1);
                        console.write_str(console.current_command_input);
                        console.current_command_input_cursor_pos--;
                    }
                    console.move_cursor_to_left(console.current_command_input.length() - console.current_command_input_cursor_pos);
                    ///
                    break;
                case Console::SPACE:
                    console.current_command_input += c;
                    console.current_command_input_cursor_pos++;
                    break;
                case 27:{
                    //ESC
                    char ch_1 = Console::Instance().getkey();
                    char ch_2 = Console::Instance().getkey();
                    auto esc_sequence = to_esq_sequence(ch_1, ch_2);
                    switch (esc_sequence){
                        case ESC_sequense::ARROW_LEFT:
                            console.move_cursor_to_left(console.current_command_input_cursor_pos + 4);
                            console.clear_line_from_cursor_position();
                            console.write_str(console.current_command_input);
                            if (console.current_command_input_cursor_pos != 0){
                                console.current_command_input_cursor_pos--;
                            }
                            console.move_cursor_to_left(console.current_command_input.length() - console.current_command_input_cursor_pos);
                            break;
                        case ESC_sequense::ARROW_RIGHT:
                            console.move_cursor_to_left(console.current_command_input_cursor_pos + 4);
                            console.clear_line_from_cursor_position();
                            console.write_str(console.current_command_input);
                            if (console.current_command_input_cursor_pos < console.current_command_input.length()){
                                console.current_command_input_cursor_pos++;
                            }
                            console.move_cursor_to_left(console.current_command_input.length() - console.current_command_input_cursor_pos);
                            break;
                    }
                    break;
                }
                default:
                    console.current_command_input += c;
                    console.current_command_input_cursor_pos++;
                    break;
            }
        }
    }
}
