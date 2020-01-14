//
// Created by 김현규 on 2020/01/11.
//

#include "Controller.h"
#include "Engine/FuzzEngine.h"
#include "Engine/RegexEngine.h"

#include <iostream>
#include <future>

namespace {
    std::wstring s2ws(const std::string &str) {
        std::wstring wstrTo(str.begin(), str.end());
        return wstrTo;
    }
} //unnamed namespace

std::wstring test_path = L"/Volumes/";

void Controller::Initialize() {
    engine_ = std::unique_ptr<IEngine>((IEngine *) new RegexEngine);
    fileEnumerator_ = std::unique_ptr<FileEnumerator>(new FileEnumerator(test_path));
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
    std::thread{&IEngine::Start, engine_.get(), L"[A-Z]*.cpp", taskList_.get()}.detach();
    std::thread{&View::Run, &view_}.detach();

    auto start = std::chrono::system_clock::now();

    int idx = 0;
    while (1) {
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
        auto a = engine_->GetResult();
        if (a.size() > 10) {
            for (int i = 0; i < 1; i++)
            {
                std::wcout << a.top().second << '\n';
                a.pop();
            }
        }
        */
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        /*
        if (idx == 10) {
            engine_->Stop();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        //	std::thread{ &IEngine::Run, engine_.get(), L".*.cpp", taskList_.get() }.detach();
        }
        */
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    //std::cout << diff.count() << std::endl;
}

void Controller::EnumeratorCallback(Chunk chunk) {
    //std::lock_guard lk(fileMutex_);
    //std::cout << "notify : " << taskList_->size() << ' ' << chunk.size() << '\n';
    if (engineOn_)
        engine_->Query(chunk);
    taskList_->push_back(std::move(chunk));
}

void Controller::ViewCallback(std::string inputStr) {
    engine_->Stop();
    engineOn_ = false;

    std::thread{&IEngine::Start, engine_.get(), s2ws(inputStr), taskList_.get()}.detach();
    engineOn_ = true;
    for (const auto &task : *taskList_)
        engine_->Query(task);
}
