cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)


project(TFS C CXX)

set(TFS_VERSION 1.0)

set(tfs_lib_SRC tfs.cc)

message(INFO "yocto = ${yocto}")

if("${yocto}" STREQUAL "")
    add_subdirectory(kmod)
else()
    add_subdirectory(lib)
    add_subdirectory(tests)
    add_subdirectory(kmod)
    add_subdirectory(mkfs)
endif()



