cmake_minimum_required (VERSION 3.0)
project (TJoin)

option(ENABLE_TESTS "Enable test execution." OFF)

# Setup testing
enable_testing()
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(include)

# Add test cpp file
add_executable(BinaryHeapTests
    tests/BinaryHeapTests.cpp
)

# Link test executable against gtest & gtest_main
target_link_libraries(BinaryHeapTests
    ${GTEST_BOTH_LIBRARIES} 
    ${CMAKE_THREAD_LIBS_INIT}
)

if (ENABLE_TESTS)
    set(UNIT_TEST BinaryHeapTests)
    add_custom_command(
         TARGET ${UNIT_TEST}
         COMMENT "Run Binary Heap tests"
         POST_BUILD 
         COMMAND ${UNIT_TEST}
    )
endif()
