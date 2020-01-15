//
// Created by 김현규 on 2020/01/14.
//

#include "ThreadPool.h"
#include <iostream>

ThreadPool::ThreadPool(size_t threads)
        : threadpoolStopped_(false),
          threadpoolPaused_(false),
          isWorking_(threads, false) {
    auto a = [this] {
        for (;;) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(this->taskMutex_);
                //! 스레드풀이 종료되었거나 Task큐가 비지 않았다면 일어나지 않는다

                this->condition_.wait(lock, [this]() {
                    return this->threadpoolStopped_ || !this->tasks_.empty();
                });

                //! graceful shutdown을 위한 코드
                if (this->threadpoolStopped_ && this->tasks_.empty()) {
                    return;
                }
                task = std::move(this->tasks_.front());
                this->tasks_.pop();
            }
            //! Pause함수가 잘 동작하기 위해서는 각 스레드가 작업중인지를
            //! 적절히 판단할 수 있어야 한다.
            this->isWorking_[0] = true;
            task();
            this->isWorking_[0] = false;
            if (this->threadpoolPaused_) {
                paused_.notify_one();
            }
        }
    };
    for (size_t i = 0; i < threads; ++i)
        workers_.emplace_back(a);
}

void ThreadPool::Pause() {
    //! Pause, Resume, AddTask는 동시에 실행되서는 안된다.
    std::unique_lock<std::mutex> lock(interfaceMutex_);
    //std::cout << "pause called";
    {
        std::unique_lock<std::mutex> lock(taskMutex_);
        while (!tasks_.empty()) {
            tasks_.pop();
        }
    }

    //! 모든 스레드의 작업이 종료될 때까지 기다린다.
    //! 각 Task가 너무 오랜 시간 걸린다면, 오랜시간 Blocking될 수 있다.
    //! TODO(김현규) : 타임아웃을 두어서 행이 걸리는 것을 방지하자.
    this->paused_.wait(lock, [this] {
        for (const auto &threadWorking : this->isWorking_) {
            if (threadWorking)
                return false;
        }
        return true;
    });
    threadpoolPaused_ = true;
}

void ThreadPool::Resume() {
    std::unique_lock<std::mutex> lock(interfaceMutex_);
    threadpoolPaused_ = false;
}


//! 실행중인 모든 스레드가 작업이 끝날 때 까지 기다린 후 종료한다.
//! 이 때는 그냥 join해도 충분하다.
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(taskMutex_);
        threadpoolStopped_ = true;
    }
    condition_.notify_all();
    for (std::thread &worker : workers_)
        worker.join();
}
