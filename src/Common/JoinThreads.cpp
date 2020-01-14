//
// Created by 김현규 on 2020/01/14.
//

#include "JoinThreads.h"


JoinThreads::JoinThreads(std::vector<std::thread> &threads_) :
        threads(threads_) {}

JoinThreads::~JoinThreads() {
    for (unsigned long i = 0; i < threads.size(); ++i) {
        if (threads[i].joinable())
            threads[i].join();
    }
}
