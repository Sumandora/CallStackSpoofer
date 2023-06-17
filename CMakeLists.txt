cmake_minimum_required(VERSION 3.20)
project(CallStackSpoofer)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -rdynamic")

add_executable(CallStackSpoofer Example/Main.cpp Source/CallStackSpoofer.cpp)
target_include_directories(CallStackSpoofer PRIVATE include)
target_compile_definitions(CallStackSpoofer PUBLIC JMP_REGISTER="rbx") # Possible registers: rbx, r12, r13, r14, and r15
