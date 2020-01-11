//
// Created by 김현규 on 2020/01/11.
//

#include "FileEnumerator.h"
#include <iostream>


FileEnumerator::FileEnumerator(std::wstring rootDirectory, IEngine *engine)
        : rootDirectory_(std::move(rootDirectory)),
          engine_(engine) {
    directoryQueue_.push(rootDirectory_);
}

void FileEnumerator::Run() {
    while (!directoryQueue_.empty()) {
        auto currentDir = directoryQueue_.front();
        directoryQueue_.pop();

        for (const auto &path : std::filesystem::directory_iterator(currentDir)) {
            if (std::filesystem::is_directory(path)) {
                directoryQueue_.push(path.path().wstring());
            } else {
                std::lock_guard<std::mutex> lk(taskMutex_);
                task_.AddPath(path.path().wstring());

                if (task_.GetSize() == kTaskSize_) {
                    Notify();
                }
            }
        }
    }
}

void FileEnumerator::Notify() {
    std::cout << "FileEnumerator::Notify() called!" << '\n';
    engine_->AddTask(std::move(task_));
}
