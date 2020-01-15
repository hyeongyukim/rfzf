//
// Created by 김현규 on 2020/01/11.
//

#include "RegexEngine.h"
#include <iostream>
#include <thread>
#include <regex>
#include <mutex>


RegexEngine::~RegexEngine() {
}

void RegexEngine::ProcessChunk(const Chunk chunk) {
    std::wsmatch m;
    static int i = 0;
    //std::wcout << L"processed_ " << chunk.size() << str_ << '\n';
    try {
        std::wregex pat(str_);
        for (const auto &path : chunk) {
            processed_++;
            if (std::regex_match(path, m, pat)) {
                // regex_match가 성공할 가능성이 크지 않다고 판단하여
                // 내부에서 잠금
                std::lock_guard<std::mutex> lk(mutexForWorker_);
                res_.emplace(std::make_pair(path.size(), path));
                if (res_.size() > 60)
                    res_.pop();
            }
        }
    } catch (...) {
        //std::wcout << "error!" << '\n';
    }
}

/*
 *
 * .*.h
 */
std::unique_ptr<IEngine> RegexEngine::CreateEngine() {
    return std::make_unique<RegexEngine>(_construct_token{});
}

void RegexEngine::Start(std::wstring str) {
    std::lock_guard<std::mutex> lock(mutexForInstance_);
    str_ = std::move(str);
    processed_ = 0;
    running_ = true;
    idx_ = 0;

    {
        std::lock_guard<std::mutex> lk(mutexForWorker_);
        while (!res_.empty()) {
            res_.pop();
        }
    }
    threadPool_.Resume();
}

void RegexEngine::Stop() {
    std::lock_guard<std::mutex> lock(mutexForInstance_);
    threadPool_.Pause();
    running_ = false;
}

ResultList RegexEngine::GetResult() {
    std::lock_guard<std::mutex> lk(mutexForWorker_);
    return res_;
}

void RegexEngine::Query(Chunk chunk) {
    //std::lock_guard<std::mutex> lock(mutexForInstance_);
    while (!running_)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    threadPool_.AddTask(std::bind(&RegexEngine::ProcessChunk, this, chunk));
}

uint32_t RegexEngine::GetStatics() {
    std::lock_guard<std::mutex> lock(mutexForInstance_);
    return processed_;
}
