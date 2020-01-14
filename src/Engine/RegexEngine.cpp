//
// Created by 김현규 on 2020/01/11.
//

#include "RegexEngine.h"
#include <iostream>
#include <thread>
#include <regex>
#include <mutex>
#include <boost/asio.hpp>


RegexEngine::~RegexEngine() {
}

void RegexEngine::ProcessChunk(const Chunk chunk) {
    std::wsmatch m;
    try {
        std::wregex pat(str_);
        for (const auto &path : chunk) {
            processed_++;
            if (std::regex_search(path, m, pat)) {
                //std::this_thread::sleep_for(std::chrono::microseconds(100));
                std::lock_guard<std::mutex> lk(mutexForWorker_);
                res_.emplace(std::make_pair(path.size(), path));
                if (res_.size() > 100)
                    res_.pop();
            }
        }
    } catch (...) {
        std::lock_guard<std::mutex> lk(mutexForWorker_);
        while (!res_.empty()) {
            res_.pop();
        }
    }
}

void RegexEngine::Start(std::wstring str, const TaskList *taskList) {
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


    /*
    while (true)
    {
        //std::unique_lock lk(mutexForWorker_);

        cvEvent_.wait(lk, [this, taskList]() {
            return !running_ || taskList->size() > idx_;
        });
        auto a = std::chrono::system_clock::now();
        std::cout << a.time_since_epoch().count() << "unlocked\n";

        // 엔진이 종료된 경우, 루프를 빠져 나온다.
        if (!running_) {
            break;
        }

        //try {
            if(idx_ < taskList->size())
                ProcessChunk((*taskList)[idx_++]);
        //}
        //catch(...) {}
    }
    */
}

void RegexEngine::Stop() {
    running_ = false;
}

ResultList RegexEngine::GetResult() {
    std::lock_guard<std::mutex> lk(mutexForWorker_);
    return res_;
}

void RegexEngine::Query(Chunk chunk) {
    //boost::asio::post(pool, std::bind(&RegexEngine::ProcessChunk, this, (std::move(chunk))));
//	waitingQueue_.push(std::move(chunk));
    pool2.enqueue(&RegexEngine::ProcessChunk, this, (std::move(chunk)));
}

uint32_t RegexEngine::GetStatics() {
    return processed_;
}

/*
void RegexEngine::TaskAdded()
{
	auto a = std::chrono::system_clock::now();
	std::cout << a.time_since_epoch().count() << "added\n";
	cvEvent_.notify_one();
}
*/
