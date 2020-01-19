//
// Created by 김현규 on 2020/01/11.
//

#ifndef RFZF_IENGINE_H
#define RFZF_IENGINE_H

#include <queue>

#include "DataType.h"

class IEngine {
public:
    virtual ~IEngine() = default;

    virtual bool Start(std::wstring str) = 0;

    virtual bool Stop() = 0;

    virtual ResultList GetResult() = 0;

    virtual bool Query(Chunk *chunk) = 0;

    virtual uint32_t GetProcessedNum() = 0;
};

#endif //RFZF_IENGINE_H
