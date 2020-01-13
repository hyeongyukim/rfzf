//
// Created by 김현규 on 2020/01/11.
//

#include "Controller.h"
#include "FuzzEngine.h"
#include "RegexEngine.h"

#include <iostream>

std::wstring test_path = L"/Volumes/Workingspace";

void Controller::Initialize() {
    engine_ = std::unique_ptr<IEngine>((IEngine *) new RegexEngine);
    fileEnumerator_ = std::unique_ptr<FileEnumerator>(new FileEnumerator(test_path));
}

void Controller::Run() {
    engine_->Run();
    fileEnumerator_->RegisterCallback(std::bind(
            &Controller::EnumeratorCallback,
            this,
            std::placeholders::_1));
    fileEnumerator_->Run();
}

void Controller::EnumeratorCallback(Chunk chunk) {
    std::lock_guard lk(fileMutex_);
    fileList_.push_back(chunk);

    std::cout << fileList_.size() << '\n';
    for (auto &a : chunk) {
        std::wcout << a << '\n';
    }
}

