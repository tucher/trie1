cmake_minimum_required(VERSION 3.0.0)
project(trie VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(trie main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
set_property(TARGET trie PROPERTY CXX_STANDARD 17)
