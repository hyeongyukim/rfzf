//
// Created by 김현규 on 2020/01/14.
//

#include "ThreadsafeQueue.h"

template<typename T>
ThreadsafeQueue<T>::ThreadsafeQueue() : head(new node), tail(head.get()) {}

template<typename T>
std::shared_ptr<T> ThreadsafeQueue<T>::try_pop() {
    std::unique_ptr<node> old_head = pop_head();
    return old_head ? old_head->data : std::shared_ptr<T>();
}

template<typename T>
void ThreadsafeQueue<T>::push(T new_value) {
    std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
    std::unique_ptr<node> p(new node);
    node *const new_tail = p.get();
    std::lock_guard<std::mutex> tail_lock(tail_mutex);
    tail->data = new_data;
    tail->next = std::move(p);
    tail = new_tail;
}

template<typename T>
typename ThreadsafeQueue<T>::node *ThreadsafeQueue<T>::get_tail() {
    std::lock_guard<std::mutex> tail_lock(tail_mutex);
    return tail;;
}

template<typename T>
std::unique_ptr<typename ThreadsafeQueue<T>::node> ThreadsafeQueue<T>::pop_head() {
    std::lock_guard<std::mutex> head_lock(head_mutex);

    if (head.get() == get_tail()) {
        return nullptr;
    }
    std::unique_ptr<node> old_head = std::move(head);
    head = std::move(old_head->next);
    return old_head;
}
