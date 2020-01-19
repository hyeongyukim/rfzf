//
// Created by 김현규 on 2020/01/11.
//

#ifndef RFZF_REGEXENGINE_H
#define RFZF_REGEXENGINE_H

#include <atomic>
#include <regex>
#include <mutex>

#include "Common/ThreadPool.h"
#include "IEngine.h"
#include "DataType.h"
#include "spdlog/spdlog.h"

class RegexEngine : public IEngine {
private:
    //! Named Constructor Idiom을 사용할 때는 직접 객체를 생성하는 것을 방지하여야 한다.
    //! 생성자를 private으로 선언하는 것이 일반적이지만, make_unique, make_shared등의
    //! 함수를 호출할 수 없으므로, _construct_token을 이용하여 팩토리에서만 생성 가능하게
    //! 만들 수 있도록 하였다.
    struct _construct_token {
    };

public:
    //! @brief: RegexEngine의 생성자.
    //! @param threadnum: regex matching에 사용할 스레드 수
    //! @param _construct_token: Named Constructor Idiom을 위한 토큰
    explicit RegexEngine(_construct_token, int threadNum);

    ~RegexEngine() override = default;

    //! @brief: RegexEngine을 생성해주는 팩토리 함수
    //! @param threadnum: regex matching에 사용할 스레드 수
    static std::unique_ptr<IEngine> CreateEngine(int threadNum);

    //! @brief: 정규 표현식 기반으로 문자열을 매칭한다. 매칭한 결과는 GetResult()함수로
    //!         pooling해서 사용한다.
    //! @param str: 정규 표현식에 사용할 문자열
    bool Start(std::wstring str) override;

    //! @brief: 엔진의 동작을 정지한다.
    bool Stop() override;

    //! @brief: 매칭할 문자열 큐에 새로운 문자열 Chunk를 추가한다.
    //! @param chunk: 문자열이 저장된 Chunk를 가리키는 포인터, rfzf에서는 Chunk객체의
    //!               생명주기는 외부에서 관리하므로 raw포인터를 인자로 받는다.
    //!               만약, chunk가 소멸될 가능성이 있다면, shared_ptr<Chunk>등으로
    //!               변경해야 한다.
    bool Query(Chunk *chunk) override;

    //! @brief: 현재까지 매칭한 결과를 반환하는 함수.
    ResultList GetResult() override;

    //! @brief: 현재까지 매칭된 문자열의 수를 반환하는 함수, rfzf하단에 표시된다.
    uint32_t GetProcessedNum() override;

private:

    //! @brief: regex-matching을 수행하는 함수, ThreadPool의 인자로 넘겨진다.
    //! @param chunk: regex-matching의 대상이 되는 객체의 포인터
    void ProcessChunk(Chunk *chunk);

    //! 현재까지 계산된 결과를 저장할 리스트, fuzzy-find특성상 가장 짧은 길이의 문자열을
    //! 반환해야 하므로 priority_queue를 사용하였다.
    ResultList res_;
    constexpr static uint32_t maxResultListSize_ = 100;

    //! regex-matching에 사용될 문자열
    std::wregex pat_;
    std::wstring str_;

    //! 처리된 항목의 개수
    uint32_t processed_;

    bool running_;

    //! 한번에 하나의 public 함수만 실행되도록 만드는 뮤텍스
    std::mutex mutexForInstance_;
    std::mutex mutexForWorker_;

    ThreadPool threadPool_;

    std::shared_ptr<spdlog::logger> fileLogger_;

};

#endif //RFZF_REGEXENGINE_H
