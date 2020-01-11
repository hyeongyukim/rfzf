//
// Created by 김현규 on 2020/01/11.
//
#ifndef RFZF_FILEENUMERATOR_H
#define RFZF_FILEENUMERATOR_H

#include <boost/filesystem.hpp>
#include <string>
#include <queue>
#include <vector>
#include <mutex>

#include "IEngine.h"
#include "Task.h"

class FileEnumerator {
public:
    explicit FileEnumerator(std::wstring rootDirectory, IEngine *engine);

    void Run();

private:

    void Notify();

    static constexpr uint32_t kTaskSize_ = 10;

    std::wstring rootDirectory_;
    IEngine *engine_;
    std::queue<std::wstring> directoryQueue_;
    Task task_;
    std::mutex taskMutex_;

};

#endif //RFZF_FILEENUMERATOR_H
