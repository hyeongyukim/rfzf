//
// Created by 김현규 on 2020/01/13.
//

#ifndef RFZF_DATATYPE_H
#define RFZF_DATATYPE_H

#include <functional>
#include <filesystem>

using Chunk = std::vector<std::wstring>;
using t_notify = std::function<void(Chunk)>;
namespace fs = std::filesystem;

#endif //RFZF_DATATYPE_H
