cmake_minimum_required(VERSION 3.14)
project(rfzf)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(include)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")

add_subdirectory(extern/spdlog)

find_package(spdlog)

SET(SRC_FILES src/main.cpp
        src/Enumerator/FileEnumerator.h
        src/Enumerator/FileEnumerator.cpp
        src/Engine/IEngine.h
        src/Engine/FuzzEngine.h
        src/Engine/RegexEngine.h
        src/Controller.h
        src/Controller.cpp
        src/Engine/FuzzEngine.cpp
        src/Engine/RegexEngine.cpp
        src/View.h
        src/View.cpp
        include/DataType.h
        src/Common/ThreadPool.cpp
        src/Common/ThreadPool.h
        )

add_executable(rfzf ${SRC_FILES})

include_directories(extern/spdlog/include
        include
        src)

target_link_libraries(rfzf spdlog)
