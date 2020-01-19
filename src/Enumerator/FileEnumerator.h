//
// Created by 김현규 on 2020/01/11.
//
#ifndef RFZF_FILEENUMERATOR_H
#define RFZF_FILEENUMERATOR_H

#include <string>
#include <queue>
#include <vector>
#include <mutex>
#include <atomic>

#include "DataType.h"

class FileEnumerator {
private:
    struct _construct_token {
    };
public:
    explicit FileEnumerator(_construct_token, std::wstring rootDirectory);

    static std::unique_ptr<FileEnumerator> CreateFileEnumerator(std::wstring rootDirectory);

    void RegisterCallback(t_notify notify);

    void Run();

    uint32_t GetStatics();

private:
    void Notify();

    void Enumerate(std::wstring directory);

    static constexpr uint32_t kTaskSize_ = 100;

    std::wstring rootDirectory_;

    std::queue<std::wstring> directoryQueue_;

    std::vector<t_notify> callbacks_;

    Chunk chunk_;

    std::mutex chunkMutex_;
    std::atomic<uint32_t> processed_;
};

#endif //RFZF_FILEENUMERATOR_H
