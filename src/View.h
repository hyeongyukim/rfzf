//
// Created by 김현규 on 2020/01/12.
//

#ifndef RFZF_VIEW_H
#define RFZF_VIEW_H

#include <string>
#include <vector>
#include "DataType.h"

#include <unistd.h> // for STDOUT_FILENO
#include <termios.h>
#include <mutex>

class View {
public:
    View();

    ~View();

    void Print(std::vector<std::wstring> &paths, int a, int b);

    void Run();

    void RegisterCallback(t_Notify_View callback);

private:
    void EnableRawMode() noexcept;

    void DisableRawMode() noexcept;

    void Notify();

    void RestoreCursor();

    struct termios origTermios_;
    std::mutex drawLock_;
    std::string inputStr_;

    std::pair<int, int> cursorLoc_;

    std::vector<t_Notify_View> callbacks_;
};

#endif //RFZF_VIEW_H
