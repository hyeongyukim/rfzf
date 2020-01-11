//
// Created by 김현규 on 2020/01/11.
//

#ifndef RFZF_REGEXENGINE_H
#define RFZF_REGEXENGINE_H

#include "IEngine.h"

class RegexEngine : IEngine {
public:
    ~RegexEngine() override;

    void Run() override;

    virtual void AddTask(Task &&task) override;

private:
    std::queue<Task> taskQueue_;
};

#endif //RFZF_REGEXENGINE_H
