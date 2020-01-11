//
// Created by 김현규 on 2020/01/11.
//

#include "Task.h"

void Task::AddPath(std::wstring path) {
    paths_.push_back(std::move(path));
}

uint32_t Task::GetSize() {
    return paths_.size();
}