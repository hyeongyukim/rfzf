//
// Created by 김현규 on 2020/01/11.
//

#ifndef RFZF_IENGINE_H
#define RFZF_IENGINE_H

#include <queue>

#include "DataType.h"

class IEngine {
public:
	virtual ~IEngine() {};

	virtual void Start(std::wstring str, const TaskList* taskList) = 0;

	virtual void Stop() = 0;

	virtual ResultList GetResult() = 0;

	virtual void Query(Chunk chunk) = 0;

	virtual uint32_t GetStatics() = 0;


private:
};

#endif //RFZF_IENGINE_H
