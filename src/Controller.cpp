//
// Created by 김현규 on 2020/01/11.
//

#include "Controller.h"
#include "FuzzEngine.h"
#include "RegexEngine.h"

std::wstring test_path = L"/Volumes/Workingspace/flag";

void Controller::Initialize() {
    engine_ = std::unique_ptr<IEngine>((IEngine *) new RegexEngine);
    fileEnumerator_ = std::unique_ptr<FileEnumerator>(new FileEnumerator(test_path, engine_.get()));
}

void Controller::Run() {
    fileEnumerator_->Run();
    engine_->Run();
}

