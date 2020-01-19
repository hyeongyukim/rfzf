
#include <iostream>
#include "Controller.h"
#include "spdlog/sinks/basic_file_sink.h"

int main() {
    std::wcout.imbue(std::locale(""));

    auto enumerator = FileEnumerator::CreateFileEnumerator(L"/Volumes/");
    auto regexEngine = RegexEngine::CreateEngine(2);
    auto control = Controller::Create(std::move(regexEngine), std::move(enumerator));

    control->Initialize();
    control->Run();

    return 0;
}

