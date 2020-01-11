//
// Created by 김현규 on 2020/01/11.
//

#include "RegexEngine.h"
#include <iostream>

void RegexEngine::Run() {
    std::cout << "RegexEngine running!" << '\n';
}

void RegexEngine::AddTask(Task &&task) {
    std::cout << "task added!" << '\n';
    std::cout << "size : " << task.GetSize() << '\n';

    for (const auto &line : task.paths_) {
        std::wcout << "line : " << line << '\n';
    }
    taskQueue_.push(std::move(task));
}

RegexEngine::~RegexEngine() {
    std::cout << "RegexEngine Removed!" << '\n';
}

