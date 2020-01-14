//
// Created by 김현규 on 2020/01/14.
//

#ifndef RFZF_JOINTHREADS_H
#define RFZF_JOINTHREADS_H

#include <vector>
#include <thread>

class JoinThreads {
public:
    explicit JoinThreads(std::vector<std::thread> &threads_);

    ~JoinThreads();

private:
    std::vector<std::thread> &threads;
};

#endif //RFZF_JOINTHREADS_H
