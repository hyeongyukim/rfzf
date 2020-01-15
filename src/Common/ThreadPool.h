//
// Created by 김현규 on 2020/01/14.
//


#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include "../../include/DataType.h"

#ifndef RFZF_THREADPOOL_H
#define RFZF_THREADPOOL_H

//! 스레드 풀을 정지하는 것보다 삭제하고 새로 생성하는 것이 보다 일반적인
//! 구현이지만, 매 입력이 들어올 때마다 스레드를 삭제하고 생성하는 데 큰
//! 비용이 드므로 스레드 풀에 잠시 정지하는 기능을 추가하였다.
//! Pause할 수 있는 간단한 형태의 ThreadPool
class ThreadPool {
public:
    ThreadPool(size_t thread);

    ~ThreadPool();

    //! 복사 및 할당 금지
    RFZF_DISALLOW_COPY_AND_ASSIGN(ThreadPool)

    //! 새로운 Task를 추가하는 함수
    template<class F, class... Args>
    auto AddTask(F &&function, Args &&... arguments)
    -> std::future<typename std::result_of<F(Args...)>::type>;

    //! 스레드 풀을 잠시 정지하고, 내부에 들어있던 task들을 모두 삭제한다.
    //! 이미 실행중이던 task들이 모두 정지할때까지 blocking된다.
    void Pause();

    //! 스레드 풀을 다시 실행시킨다.
    void Resume();

private:
    //! worker thread들을 담고 있는 vector이다
    std::vector<std::thread> workers_;
    //! task들을 저장해두고 있는 벡터이다.
    std::queue<std::function<void()>> tasks_;

    //! task를 위한 뮤텍스이다.
    //! TODO(김현규) : lock-free queue를 사용하여 개선
    std::mutex taskMutex_;

    //! interface를 위한 뮤텍스이다.
    std::mutex interfaceMutex_;

    //! 새로운 task 할당 및 스레드 정지를 위한 cv
    std::condition_variable condition_;

    //! 각 스레드들이 task를 수행중인지 여부를 저장하는 벡터이다
    std::vector<bool> isWorking_;

    //! 스레드가 Task를 마쳤을 때 아래 cv를 통해 스레드가 정지되었음을 알려준다.
    //! 이 정보는 Pause()에서 모든 스레드가 정지 상태로 기다릴때 사용된다.
    std::condition_variable paused_;

    //! 스레드풀의 상태를 관리하기 위한 변수이다.
    std::atomic<bool> threadpoolPaused_;
    std::atomic<bool> threadpoolStopped_;
};

template<class F, class... Args>
auto ThreadPool::AddTask(F &&function, Args &&... arguments)
-> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;

//    std::unique_lock<std::mutex> lock(interfaceMutex_);

    //! 입력받은 함수와 인자들을 return_type()으로 패키징한다.
    auto task = std::make_shared<std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(function), std::forward<Args>(arguments)...)
    );

    //! 패키징된 태스크의 리턴값은 future<T>로 리턴한다.
    //! runner의 입장에서는 리턴값을 몰라도 된다!
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(taskMutex_);

        //! 스레드 풀이 정지상태일때는 task를 추가할 수 없다.
        if (threadpoolStopped_) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }
        //! 인자와 리턴값이 적절히 묶여있기 때문에 void()형태로 큐에 삽입된다
        if (!threadpoolPaused_) {
            tasks_.emplace([task]() { (*task)(); });
        }
    }
    //! 한 개의 스레드를 깨운다
    condition_.notify_one();
    return res;
}


#endif //RFZF_THREADPOOL_H

