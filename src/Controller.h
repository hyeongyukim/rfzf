//
// Created by 김현규 on 2020/01/11.
//

#ifndef RFZF_CONTROLLER_H
#define RFZF_CONTROLLER_H

#include "Engine/IEngine.h"
#include "DataType.h"
#include "Enumerator/FileEnumerator.h"
#include "View.h"

class Controller {
public:
    void Initialize();

    void Run();

private:

    void EnumeratorCallback(Chunk chunk);

    void ViewCallback(std::string inputStr);

    bool engineOn_;
    std::unique_ptr<IEngine> engine_;
    std::unique_ptr<FileEnumerator> fileEnumerator_;
    std::unique_ptr<TaskList> taskList_;
    View view_;

    std::mutex fileMutex_;
    std::vector<Chunk> fileList_;
};

#endif //RFZF_CONTROLLER_H
