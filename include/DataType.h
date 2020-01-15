//
// Created by 김현규 on 2020/01/13.
//

#ifndef RFZF_DATATYPE_H
#define RFZF_DATATYPE_H

#include <functional>
#include <filesystem>
#include <queue>

using Chunk = std::vector<std::wstring>;
using TaskList = std::vector<Chunk>;
using t_notify = std::function<void(Chunk)>;
using t_Notify_View = std::function<void(std::string)>;
namespace fs = std::filesystem;
using ResultList = std::priority_queue<std::pair<int, std::wstring>,
        std::vector<std::pair<int, std::wstring>>,
        std::less<std::pair<int, std::wstring>>>;

struct WinSize {
    uint16_t y, x;
};

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#define __MAC_OS_X__
#endif

//
#define RFZF_DISALLOW_COPY_AND_ASSIGN(class_name)    \
class_name(class_name&)                  = delete;  \
class_name& operator=(const class_name&) = delete;

//
/*
#define RFZF_ADD_TYPE(_FUNC_NAME_, _DERIVED_TYPE_)  \
virtual void _FUNC_NAME_(_DERIVED_TYPE_* node) { return; }
*/

//
//#define __FUNC__ void*

#endif //RFZF_DATATYPE_H
