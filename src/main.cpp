#include <iostream>
#include "Controller.h"

#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h> // for STDOUT_FILENO
#include <termios.h>
#include <ctype.h>


struct termios orig_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_iflag &= ~(ICRNL | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


int main() {
    Controller control;

    control.Initialize();
    control.Run();

    /*
    std::string str;
    std::cout << str << std::endl;

    enableRawMode();

    struct winsize size;
    system("clear");
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    printf("\033[%d;%dH\n", size.ws_row, 1);

    char c;
    int cnt = 1;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        if (c == 127) {
            str.pop_back();
        } else if (iscntrl(c)) {
            break;
        } else {
            str.push_back(c);
        }

        printf("\033[%d;%dH", size.ws_row, 1);
        printf("%s", str.c_str());
        for (int i = 0; i < size.ws_col - str.size(); i++)
            printf(" ");
        printf("\033[%d;%dH\n", size.ws_row - 1, str.size() + 1);
    }
    */
    return 0;
}
