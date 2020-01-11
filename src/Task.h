//
// Created by 김현규 on 2020/01/11.
//

#ifndef RFZF_TASK_H
#define RFZF_TASK_H

#include <vector>
#include <string>

class Task {
public:
    void AddPath(std::wstring path);

    uint32_t GetSize();

    std::vector<std::wstring> paths_;
};

#endif //RFZF_TASK_H
