#include "Console.hpp"

#include <cstdio>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

Console::Console(){
    _in_descriptor = fileno(stdin);
    _out_descriptor = fileno(stdout);
}

Console& Console::Instance(){
    static Console *singleton = nullptr;
    static std::mutex mutex;
    if (!singleton){
        std::lock_guard<std::mutex> guard(mutex);
        if (!singleton){
            singleton = new Console();
        }
    }
    return *singleton;
}

int Console::getkey(){
    // https://programmersought.com/article/50833994785/
    // https://stackoverflow.com/a/14323342
    char ch;
    int error;
    struct termios oldAttr, newAttr;
    int oldFlags, newFlags;
    struct timeval tv;
    int fd = fileno(stdin);
    tcgetattr(fd, &oldAttr);
    newAttr = oldAttr;
    oldFlags = fcntl(fd, F_GETFL, 0);

    newAttr.c_iflag = 0; /* input mode */
    newAttr.c_oflag = 0; /* output mode */
    newAttr.c_lflag &= ~ICANON; /* line settings */
    newAttr.c_cc[VMIN] = 1; /* minimum chars to wait for */
    newAttr.c_cc[VTIME] = 1; /* minimum wait time */

    // Set stdin to nonblocking, noncanonical input
    fcntl(fd, F_SETFL, O_NONBLOCK);
    error=tcsetattr(fd, TCSANOW, &newAttr);

    tv.tv_sec = 0;
    tv.tv_usec = 1000; // small 0.01 msec delay //удалить ноль и будет норм
    select(1, NULL, NULL, NULL, &tv);

    if (error == 0)
        error=(read(fd, &ch, 1) != 1); // get char from stdin

    // Restore original settings
    error |= tcsetattr(fd, TCSANOW, &oldAttr);
    fcntl(fd, F_SETFL, oldFlags);

    return (error ? -1 : (int) ch);
}


void Console::move_cursor_to_left(int number){
    if (number == 0){
        return;
    }
    std::string control_sequence = "\033[" + std::to_string(number) + "D";
    write_str(control_sequence);
}


void Console::move_cursor_to_right(int number){
    if (number == 0){
        return;
    }
    std::string control_sequence = "\033[" + std::to_string(number) + 'C';
    write_str(control_sequence);
}

void Console::clear_line_from_cursor_position(){
    std::string control_sequence = "\033[K";
    write(_out_descriptor, control_sequence.c_str(), control_sequence.length());
}

void Console::write_str(const std::string& str){
    if (str.size() == 0){
        return;
    }
    write(_out_descriptor, str.c_str(), str.length());
}

void Console::change_text_color(Color color){
    current_text_color = color;
    std::string control_sequence = "\033[3" + std::to_string(static_cast<int>(color)) + "m";
    write_str(control_sequence);
}

void print_hints(const std::vector<std::string>& hints){

}
