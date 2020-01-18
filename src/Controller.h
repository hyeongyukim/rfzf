//
// Created by 김현규 on 2020/01/11.
//

#ifndef RFZF_CONTROLLER_H
#define RFZF_CONTROLLER_H

#include "Engine/IEngine.h"
#include "DataType.h"
#include "Enumerator/FileEnumerator.h"
#include "Engine/RegexEngine.h"
#include "View.h"

//! RFZF의 여러 객체들을 관리하는 주체이다.
class Controller {
public:
    Controller() = default;

    ~Controller() = default;

    RFZF_DISALLOW_COPY_AND_ASSIGN(Controller)

    /*
    static std::unique_ptr<Controller> Create(
        std::unique_ptr<IEngine> engine,
        std::unique_ptr<FileEnumerator> enumerator
    );
    */

    void Initialize();

    void Run();

private:

    void EnumeratorCallback(Chunk chunk);

    void ViewCallback(std::string inputStr);

    bool PrintScreen();

    bool engineOn_;
    std::unique_ptr<IEngine> engine_;
    std::unique_ptr<FileEnumerator> fileEnumerator_;
    TaskList taskList_;
    View view_;

    std::mutex mutexForInterface_;

    std::mutex fileMutex_;
    std::vector<Chunk> fileList_;
};

#endif //RFZF_CONTROLLER_H
