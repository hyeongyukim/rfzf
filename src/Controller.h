//
// Created by 김현규 on 2020/01/11.
//

#ifndef RFZF_CONTROLLER_H
#define RFZF_CONTROLLER_H

#include "IEngine.h"
#include "FileEnumerator.h"

class Controller {
public:
    void Initialize();

    void Run();

private:
    std::unique_ptr<IEngine> engine_;
    std::unique_ptr<FileEnumerator> fileEnumerator_;
};

#endif //RFZF_CONTROLLER_H
