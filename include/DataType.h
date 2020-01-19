//
// Created by 김현규 on 2020/01/13.
//

#ifndef RFZF_DATATYPE_H
#define RFZF_DATATYPE_H

#include <functional>
#include <filesystem>
#include <queue>

namespace fs = std::filesystem;

using Chunk = std::vector<std::wstring>;
using TaskList = std::vector<std::unique_ptr<Chunk>>;
using t_notify = std::function<void(Chunk)>;
using t_Notify_View = std::function<void(std::string)>;

using ResultList = std::priority_queue<std::pair<int, std::wstring>,
        std::vector<std::pair<int, std::wstring>>,
        std::less<>>;

struct WinSize {
    uint16_t y, x;
};

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#define __MAC_OS_X__
#endif

//
#define RFZF_DISALLOW_COPY_AND_ASSIGN(class_name)     \
class_name(class_name&)                  = delete;    \
class_name& operator=(const class_name&) = delete;


#endif //RFZF_DATATYPE_H
