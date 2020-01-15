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

//! Thread-unsafe
//! TODO(김현규) : 만약 Enumerate하는 부분이 병목이 된다면, lock-free queue등을 사용해서
//!               Thread-safe하게 수정하자.
class FileEnumerator {
public:
    explicit FileEnumerator(std::wstring rootDirectory);

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

//    boost::asio::thread_pool pool;
};

#endif //RFZF_FILEENUMERATOR_H
