//
// Created by 김현규 on 2020/01/11.
//

#ifndef RFZF_REGEXENGINE_H
#define RFZF_REGEXENGINE_H

#include "IEngine.h"
#include "DataType.h"
#include <atomic>
#include <mutex>
#include <boost/asio/thread_pool.hpp>
#include "../Common/ThreadPool.h"

class RegexEngine : IEngine {
public:
    ~RegexEngine() override;

    RegexEngine() : pool(6), pool2(6) {}

    void Start(std::wstring str, const TaskList *taskList) override;

    void Stop() override;

    ResultList GetResult() override;

    void Query(Chunk chunk) override;

    uint32_t GetStatics() override;

private:
    void ProcessChunk(const Chunk chunk);

    ResultList res_;
    std::wstring str_;
    std::atomic<uint32_t> processed_;
    std::atomic<bool> running_;
    std::atomic<uint32_t> idx_;
    std::mutex mutexForInstance_;
    std::mutex mutexForWorker_;
    std::condition_variable cvEvent_;

    std::queue<Chunk> waitingQueue_;
    boost::asio::thread_pool pool;
    ThreadPool pool2;
};

#endif //RFZF_REGEXENGINE_H
