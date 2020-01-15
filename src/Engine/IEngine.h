//
// Created by 김현규 on 2020/01/11.
//

#ifndef RFZF_IENGINE_H
#define RFZF_IENGINE_H

#include <queue>

#include "../../include/DataType.h"

class IEngine {
public:
    virtual ~IEngine() {};

    virtual void Start(std::wstring str) = 0;

    virtual void Stop() = 0;

    virtual ResultList GetResult() = 0;

    virtual void Query(Chunk chunk) = 0;

    virtual uint32_t GetStatics() = 0;
};

#endif //RFZF_IENGINE_H
