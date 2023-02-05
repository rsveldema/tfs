

set(TEST_NAME "test1")

include(CTest)

add_executable(${TEST_NAME} test1.cc)


find_package(GTest)

# Use the global target
target_link_libraries(${TEST_NAME} gtest::gtest tfs_lib)
