enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_main src/test_main.cpp)
target_compile_features(test_main PRIVATE cxx_std_17)
target_link_libraries(test_main ${GTEST_BOTH_LIBRARIES} Fluids::fluids)

add_test(AllTestsIntest_main test_main)