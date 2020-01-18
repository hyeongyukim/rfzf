//
// Created by 김현규 on 2020/01/11.
//

#ifndef RFZF_REGEXENGINE_H
#define RFZF_REGEXENGINE_H

#include "../Common/ThreadPool.h"
#include "IEngine.h"
#include "../../include/DataType.h"
#include <atomic>
#include <mutex>

class RegexEngine : public IEngine {
private:
    struct _construct_token {
    };
public:
    RegexEngine(_construct_token) : threadPool_(2) {}

    ~RegexEngine() override;

    static std::unique_ptr<IEngine> CreateEngine();

    void Start(std::wstring str) override;

    void Stop() override;

    void Query(Chunk *chunk) override;

    ResultList GetResult() override;

    uint32_t GetStatics() override;

private:

    void ProcessChunk(Chunk *chunk);

    ResultList res_;
    std::wstring str_;
    std::atomic<uint32_t> processed_;
    std::atomic<bool> running_;
    std::atomic<uint32_t> idx_;
    std::mutex mutexForInstance_;
    std::mutex mutexForWorker_;
    std::condition_variable cvEvent_;

    std::queue<Chunk> waitingQueue_;
    ThreadPool threadPool_;
};

#endif //RFZF_REGEXENGINE_H
