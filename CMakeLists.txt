cmake_minimum_required(VERSION 3.15)
project(rfzf)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(include)

SET(SRC_FILES src/main.cpp
        src/FileEnumerator.h
        src/FileEnumerator.cpp
        src/IEngine.h
        src/FuzzEngine.h
        src/RegexEngine.h
        src/Task.h
        src/Task.cpp
        src/Controller.h
        src/Controller.cpp
        src/FuzzEngine.cpp
        src/RegexEngine.cpp src/View.h src/View.cpp include/DataType.h)

add_executable(rfzf ${SRC_FILES})