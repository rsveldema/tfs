set(MY_LIB_SOURCE_DIR include)

set(tfs_lib_INC include/tfs.hh)

add_library(tfs STATIC ${tfs_lib_SRC} ${tfs_lib_INC})


install(FILES ${tfs_lib_INC} DESTINATION include)

target_include_directories(tfs PUBLIC ${MY_LIB_SOURCE_DIR})

target_compile_options(tfs PUBLIC -fno-builtin -fno-rtti -fno-exceptions -std=c++0x)
