//
// Created by 김현규 on 2020/01/12.
//

#include "View.h"
#include "DataType.h"
#include <termios.h>
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <iostream>
#include <algorithm>

namespace {
    void MoveCursor(int y, int x) noexcept {
#if defined __MAC_OS_X__
        printf("\033[%d;%dH\n", y, x);
#else
#error not implemented for linux and windows
#endif
    };
    static struct winsize winSize;
} //unnamed namespace

//#define _SIMPLE
#ifndef _SIMPLE

void View::EnableRawMode() noexcept {
    tcgetattr(STDIN_FILENO, &origTermios_);
    struct termios raw = origTermios_;
    raw.c_iflag &= ~(ICRNL | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &winSize);
    MoveCursor(winSize.ws_row - 1, 1);
}

void View::DisableRawMode() noexcept {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &origTermios_);
}

View::View() {
    EnableRawMode();
    system("clear");
    cursorLoc_.first = winSize.ws_row - 1;
    cursorLoc_.second = 1;
}

View::~View() {
    DisableRawMode();
}

void View::Print(std::vector<std::wstring> &paths, int a, int b) {
    int cursorY = 1;
    int cursorX = 1;

    auto pathIter = paths.begin();
    while (cursorY < winSize.ws_row - 2) {
        std::wstring printString = L"";
        if (pathIter != paths.end()) {
            printString = *(pathIter++);
        }

        {
            std::lock_guard lk(drawLock_);

            MoveCursor(cursorY, cursorX);
            int strSize = std::min(winSize.ws_col, uint16_t(printString.size()));
            std::wcout << printString.substr(0, strSize);
            for (int i = 0; i < winSize.ws_col - strSize; i++)
                std::wcout << ' ';
            RestoreCursor();
        }
        cursorY++;
        cursorX = 1;
    }

    {
        std::lock_guard lk(drawLock_);
        MoveCursor(winSize.ws_row - 2, 1);
        std::wcout << a << "/" << b << "             ";
        RestoreCursor();
    }
}

void View::Run() {
    char c;

    while (true) {
        if (read(STDIN_FILENO, &c, 1) != 1) {
            exit(0);
            break;
        }
        if (c == 127) {
            inputStr_.pop_back();
        } else if (iscntrl(c)) {
            exit(0);
            break;
        } else {
            inputStr_.push_back(c);
        }

        {
            std::lock_guard lk(drawLock_);
            MoveCursor(winSize.ws_row - 1, 1);
            printf("%s", inputStr_.c_str());
            for (uint i = 0; i < winSize.ws_col - inputStr_.size(); i++) {
                printf(" ");
            }
            cursorLoc_.second = (int) inputStr_.size() + 1;
            RestoreCursor();
        }
        Notify();
    }
}

void View::RestoreCursor() {
    MoveCursor(cursorLoc_.first, cursorLoc_.second);
}


#else

View::View() {

}

View::~View() {

}

void View::Print(std::vector<std::wstring> &paths, int a, int b) {
    /*
    std::cout << a << ' ' << b << ' ' << paths.size();
    if (paths.size() > 0)
        std::wcout << paths[0];
    std::cout << '\n';
     */
}

void View::Run() {
    std::string str;
    while (true) {
        std::cin >> inputStr_;
        Notify();
    }
}

void View::EnableRawMode() noexcept {

}

void View::DisableRawMode() noexcept {

}

void View::RestoreCursor() {

}

#endif

void View::Notify() {
    for (const auto &callback: callbacks_) {
        callback(inputStr_);
    }
}

void View::RegisterCallback(t_Notify_View callback) {
    callbacks_.push_back(std::move(callback));
}
