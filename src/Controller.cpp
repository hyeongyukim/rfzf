//
// Created by 김현규 on 2020/01/11.
//

#include "Controller.h"
#include "Engine/FuzzEngine.h"
#include <iostream>
#include <future>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace {

    std::vector<std::wstring> QueueToVector(ResultList &list) {
        std::vector<std::wstring> result;
        while (!list.empty()) {
            result.emplace_back(list.top().second);
            list.pop();
        }
        return result;
    }

    std::wstring s2ws(const std::string &str) {
        std::wstring wstrTo(str.begin(), str.end());
        return wstrTo;
    }

} //unnamed namespace

void Controller::Initialize() {
    filelog = spdlog::basic_logger_mt("control-logger", "log.txt");

    fileEnumerator_->RegisterCallback(std::bind(
            &Controller::EnumeratorCallback,
            this,
            std::placeholders::_1));

    view_.RegisterCallback(std::bind(
            &Controller::ViewCallback,
            this,
            std::placeholders::_1));

    std::thread{&FileEnumerator::Run, fileEnumerator_.get()}.detach();
    std::thread{&IEngine::Start, engine_.get(), L""}.detach();
    std::thread{&View::Run, &view_}.detach();
}

void Controller::Run() {
    while (true) {
        std::vector<std::wstring> resultAsVector;
        auto resultAsQueue = engine_->GetResult();
        resultAsVector = QueueToVector(resultAsQueue);

        view_.Print(resultAsVector, engine_->GetProcessedNum(), fileEnumerator_->GetStatics());
        //! TODO(김현규) : 100ms의 딜레이에 그리는 시간까지 포함되도록 수정하기
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Controller::EnumeratorCallback(Chunk chunk) {
    taskList_.emplace_back(std::make_unique<Chunk>(std::move(chunk)));
    if (engineOn_) {
        if (!engine_->Query((*taskList_.rbegin()).get())) {
            filelog->warn("EnumeratorCallback: failed to add task!");
        }
    }
}

void Controller::ViewCallback(std::string inputStr) {
    if (!engine_->Stop()) {
        filelog->warn("ViewCallback: failed to stop engine! [" + inputStr + "]");
        return;
    }
    engineOn_ = false;
    if (!engine_->Start(s2ws(inputStr))) {
        filelog->warn("ViewCallback: failed to start engine! [" + inputStr + "]");
        return;
    }
    engineOn_ = true;

    for (const auto &task : taskList_) {
        //! TODO(김현규): Query함수는 현재 단순히 스레드 풀에 Task를 추가하는 데 성공했는지
        //!             여부만 확인하고 있다. 실제 Task가 성공했는지, 실패했는지 아니면
        //!             모종의 이유로 큐에서 삭제되었는지는 알 수 없는 상황이므로, std::future을
        //!             인자로 받아 확인하는 등의 수정이 필요하다.
        if (!engine_->Query(task.get())) {
            filelog->warn("ViewCallback: failed to add task! [" + inputStr + "]");
        }
    }
}

std::unique_ptr<Controller>
Controller::Create(std::unique_ptr<IEngine> engine, std::unique_ptr<FileEnumerator> enumerator) {
    return std::make_unique<Controller>(_construct_token{}, std::move(engine), std::move(enumerator));
}

Controller::Controller(_construct_token, std::unique_ptr<IEngine> engine,
                       std::unique_ptr<FileEnumerator> enumerator) :
        engineOn_(false),
        engine_(std::move(engine)),
        fileEnumerator_(std::move(enumerator)) {
}
