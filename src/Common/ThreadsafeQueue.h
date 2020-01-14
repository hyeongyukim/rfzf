//
// Created by 김현규 on 2020/01/14.
//

#ifndef RFZF_THREADSAFEQUEUE_H
#define RFZF_THREADSAFEQUEUE_H

#include <mutex>

template<typename T>
class ThreadsafeQueue {
private:
    struct node {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };
    std::mutex head_mutex;
    std::unique_ptr<node> head;
    std::mutex tail_mutex;
    node *tail;
    std::condition_variable data_cond;
public:
    ThreadsafeQueue() :
            head(new node), tail(head.get()) {}

    ThreadsafeQueue(const ThreadsafeQueue &other) = delete;

    ThreadsafeQueue &operator=(const ThreadsafeQueue &other) = delete;

    std::shared_ptr<T> try_pop();

    bool try_pop(T &value);

    std::shared_ptr<T> wait_and_pop();

    void wait_and_pop(T &value);

    void push(T new_value);

    bool empty();
};

#endif //RFZF_THREADSAFEQUEUE_H
