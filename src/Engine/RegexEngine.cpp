//
// Created by 김현규 on 2020/01/11.
//

#include "RegexEngine.h"
#include <iostream>
#include <thread>
#include <mutex>


void RegexEngine::ProcessChunk(Chunk *chunk) {
    std::wsmatch matcher;
    for (const auto &string : *chunk) {
        processed_++;
        if (std::regex_match(string, matcher, pat_)) {
            std::lock_guard<std::mutex> lk(mutexForWorker_);
            res_.emplace(std::make_pair(string.size(), string));
            if (res_.size() > maxResultListSize_) {
                res_.pop();
            }
        }
    }
}

std::unique_ptr<IEngine> RegexEngine::CreateEngine(int threadNum) {
    return std::make_unique<RegexEngine>(_construct_token{}, threadNum);
}

bool RegexEngine::Start(std::wstring str) {
    std::lock_guard<std::mutex> lock(mutexForInstance_);
    processed_ = 0;
    running_ = true;
    {
        std::lock_guard<std::mutex> lk(mutexForWorker_);
        while (!res_.empty()) {
            res_.pop();
        }
    }

    try {
        pat_ = std::wregex(std::move(str));
    } catch (const std::regex_error &e) {
        fileLogger_->info(e.what());
        return false;
    }

    threadPool_.Resume();
    return true;
}

bool RegexEngine::Stop() {
    std::lock_guard<std::mutex> lock(mutexForInstance_);
    threadPool_.Pause();
    running_ = false;
    return true;
}

ResultList RegexEngine::GetResult() {
    std::lock_guard<std::mutex> lk(mutexForWorker_);
    return res_;
}

bool RegexEngine::Query(Chunk *chunk) {
    std::lock_guard<std::mutex> lock(mutexForInstance_);
    if (!running_) {
        return false;
    }
    threadPool_.AddTask(std::bind(&RegexEngine::ProcessChunk, this, chunk));
    return true;
}

uint32_t RegexEngine::GetProcessedNum() {
    std::lock_guard<std::mutex> lock(mutexForInstance_);
    return processed_;
}

RegexEngine::RegexEngine(RegexEngine::_construct_token, int threadNum)
        : processed_(0),
          running_(false),
          threadPool_(threadNum) {
    fileLogger_ = spdlog::basic_logger_mt("RegexEngineLogger", "Log.txt");
}
