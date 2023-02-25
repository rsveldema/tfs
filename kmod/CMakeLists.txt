

message(INFO "yocto = ${yocto}")

if("${yocto}" STREQUAL "")
  set(kerneldir "" CACHE STRING "Path to the kernel build directory")
  if("${kerneldir}" STREQUAL "")
    execute_process(COMMAND uname -r OUTPUT_VARIABLE uname_r
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(kerneldir "/lib/modules/${uname_r}/build")
  endif()

  find_file(kernel_makefile NAMES Makefile
                            PATHS ${kerneldir} NO_DEFAULT_PATH)
  if(NOT kernel_makefile)
    message(FATAL_ERROR "There is no Makefile in ${kerneldir}")
  endif()

  message(INFO "current kernel at ${kernel_makefile}")
endif()

list(APPEND module_sources tfs_module.c)

string(REPLACE ";" " " module_sources_string "${module_sources}")
string(REPLACE ".cc" ".o" module_cpp_objs_string "${tfs_lib_SRC}")
string(REPLACE ".c" ".o" module_c_objs_string "${module_sources}")


configure_file(Kbuild.in Kbuild @ONLY)

message("creating symlinks")
foreach(file "${module_sources}")
    message("mod linking ${CMAKE_CURRENT_SOURCE_DIR}/${file} ${CMAKE_CURRENT_BINARY_DIR}/${file}")
    file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/${file} ${CMAKE_CURRENT_BINARY_DIR}/${file})
endforeach()

foreach(file  "${tfs_lib_SRC}")
    message("c++ linking ${CMAKE_CURRENT_SOURCE_DIR}/../lib/${file} ${CMAKE_CURRENT_BINARY_DIR}/${file}")
    file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/../lib/${file} ${CMAKE_CURRENT_BINARY_DIR}/${file})
endforeach()


set(module_cmd ${CMAKE_MAKE_PROGRAM} V=1 -C ${kerneldir} M=${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(OUTPUT tfs_module.ko
  COMMAND ${module_cmd} modules
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${module_sources}  "${tfs_lib_SRC}" ${CMAKE_CURRENT_BINARY_DIR}/Kbuild
  VERBATIM)
add_custom_target(module DEPENDS tfs_module.ko)
add_custom_target(module-clean COMMAND ${module_cmd} clean)



