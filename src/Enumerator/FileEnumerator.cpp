//
// Created by 김현규 on 2020/01/11.
//

#include "FileEnumerator.h"
#include <iostream>
#include <boost/asio.hpp>

FileEnumerator::FileEnumerator(std::wstring rootDirectory)
        : rootDirectory_(std::move(rootDirectory)),
          processed_(0),
          pool(8) {
    directoryQueue_.push(rootDirectory_);
}

void FileEnumerator::Run() {
    while (!directoryQueue_.empty()) {
        auto currentDir = directoryQueue_.front();
        directoryQueue_.pop();
        try {
            boost::asio::post(pool, std::bind(&FileEnumerator::Enumerate, this, std::move(currentDir)));
            //Enumerate(std::move(currentDir));
        }
        catch (...) {
            //TODO(김현규) : error handling 로직 추가하기
        }
    }
    Notify();
    //std::cout << "End!!" << '\n';
}

uint32_t FileEnumerator::GetStatics() {
    return processed_;
}

void FileEnumerator::Notify() {
    // 이미 Enumberate함수 내부에서 뮤텍스를 잠근 상황이므로, 다시 잠글 필요는 없다.
    // 사실 subscriber가 하나인건 당연하므로, 옵저버 패턴 말고 콜백이 더 좋은 구조이다.(Chunk를 이동시킬 수 있다)
    // 그러나 콜백은 많이 써보아서 학습용도로 옵저버 패턴을 사용했다.
    for (const auto &callback: callbacks_) {
        callback(chunk_);
    }
    chunk_.clear();
}

void FileEnumerator::RegisterCallback(t_notify notify) {
    callbacks_.push_back(std::move(notify));
}

void FileEnumerator::Enumerate(std::wstring directory) {
    try {
        for (const auto &path : fs::directory_iterator(directory)) {
            if (std::filesystem::is_directory(path)) {
                boost::asio::post(pool, std::bind(&FileEnumerator::Enumerate, this, path.path().wstring()));
                //directoryQueue_.push(path.path().wstring());
            } else {
                processed_++;
                std::lock_guard<std::mutex> lk(chunkMutex_);
                chunk_.push_back(path.path().wstring());
                if (chunk_.size() == kTaskSize_) {
                    Notify();
                }
            }
        }
    }
    catch (...) {}
}
