//
// Created by 김현규 on 2020/01/11.
//

#include "Controller.h"
#include "Engine/FuzzEngine.h"
#include <iostream>
#include <future>

namespace {
    std::wstring s2ws(const std::string &str) {
        std::wstring wstrTo(str.begin(), str.end());
        return wstrTo;
    }
} //unnamed namespace

std::wstring test_path = L"/Volumes/Workingspace/";

void Controller::Initialize() {
    engine_ = RegexEngine::CreateEngine();
    fileEnumerator_ = std::make_unique<FileEnumerator>(test_path);
    taskList_ = std::make_unique<TaskList>();
}

void Controller::Run() {
    fileEnumerator_->RegisterCallback(std::bind(
            &Controller::EnumeratorCallback,
            this,
            std::placeholders::_1));
    view_.RegisterCallback(std::bind(
            &Controller::ViewCallback,
            this,
            std::placeholders::_1));

    std::thread{&FileEnumerator::Run, fileEnumerator_.get()}.detach();
    std::thread{&IEngine::Start, engine_.get(), L".*.cpp"}.detach();
    std::thread{&View::Run, &view_}.detach();

    auto start = std::chrono::system_clock::now();

    int idx = 0;
    while (1) {
        //auto printResult = std::async(std::bind(&Controller::PrintScreen, this), std::launch::async);
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));

        /*
        std::cout << "==================================\n";
        std::cout << idx++ << ": " << engine_->GetStatics() << '/' << fileEnumerator_->GetStatics() << "\n";
        std::cout << "==================================\n";
         */
        /*
        if (engine_->GetStatics() > 100 && engine_->GetStatics() == fileEnumerator_->GetStatics()) {
            break;
        }
         */
        std::vector<std::wstring> strs;
        auto a = engine_->GetResult();

        for (int i = 0; i < 100; i++) {
            if (a.size() == 0)
                break;
            strs.push_back(a.top().second);
            a.pop();
        }
        view_.Print(strs, engine_->GetStatics(), fileEnumerator_->GetStatics());

        /*
        auto result = engine_->GetResult();
        if (result.size() > 20) {
            for (int i = 0; i < 10; i++) {
                std::wcout << result.top().second << '\n';
                result.pop();

            }
        }
         */

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if (idx == 20) {
            engine_->Stop();
            std::thread{&IEngine::Start, engine_.get(), L"[^b]*.h"}.detach();;

            for (int i = 0; i < (*taskList_).size(); i++)
                engine_->Query((*taskList_)[i]);
        }
        if (idx == 40) {
            engine_->Stop();
            std::thread{&IEngine::Start, engine_.get(), L"[^dfghi]*.cpp"}.detach();;

            for (int i = 0; i < (*taskList_).size(); i++)
                engine_->Query((*taskList_)[i]);
        }
    }
    //auto end = std::chrono::system_clock::now();
    //std::chrono::duration<double> diff = end - start;
    //std::cout << diff.count() << std::endl;
}

void Controller::EnumeratorCallback(Chunk chunk) {
    if (engineOn_) {
        engine_->Query(chunk);
    }
    taskList_->push_back(std::move(chunk));
}

void Controller::ViewCallback(std::string inputStr) {
    //std::cout << "callback called" << '\n';
    engine_->Stop();
    engineOn_ = false;
    std::thread{&IEngine::Start, engine_.get(), s2ws(inputStr)}.detach();
    engineOn_ = true;

    for (const auto &task : *taskList_) {
        engine_->Query(task);
    }
}

bool Controller::PrintScreen() {
    return true;
}
