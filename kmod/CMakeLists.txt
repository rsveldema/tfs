


add_executable(tfs_kmod tfs_kmod.c)

target_link_libraries(tfs_kmod tfs)

target_compile_options(tfs_kmod PUBLIC -fno-builtin -nostdlib -fno-rtti -fno-exceptions -std=c++0x)


