cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)


project(TFS CXX)

set(TFS_VERSION 1.0)


add_subdirectory(lib)
add_subdirectory(tests)
add_subdirectory(kmod)



