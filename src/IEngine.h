//
// Created by 김현규 on 2020/01/11.
//

#ifndef RFZF_IENGINE_H
#define RFZF_IENGINE_H

#include <queue>

#include "Task.h"

class IEngine {
public:
    virtual ~IEngine() {}

    virtual void Run() = 0;

    virtual void AddTask(Task &&task) = 0;

private:
    std::queue<Task> Queue;
};

#endif //RFZF_IENGINE_H
