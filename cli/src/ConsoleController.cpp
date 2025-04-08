#include "ConsoleController.hpp"

#include <cstdio>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

ConsoleController::ConsoleController(int in_descriptor, int out_descriptor)
    : _in_descriptor(in_descriptor), _out_descriptor(out_descriptor)
{

}

int ConsoleController::getkey(){
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


void ConsoleController::move_cursor_to_left(int number){
    std::string control_sequence = "\033[" + std::to_string(number) + 'D';
    write(_out_descriptor, control_sequence.c_str(), control_sequence.length());
}

void ConsoleController::clear_line_from_cursor_position(){
    std::string control_sequence = "\033[K";
    write(_out_descriptor, control_sequence.c_str(), control_sequence.length());
}

void ConsoleController::write_str(const std::string& str){
    write(_out_descriptor, str.c_str(), str.length());
}

void ConsoleController::change_color(Color color){
    std::string control_sequence = "\033[3" + std::to_string(static_cast<int>(color)) + "m";
    write(_out_descriptor, control_sequence.c_str(), control_sequence.length());
}
