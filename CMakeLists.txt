cmake_minimum_required (VERSION 3.0)
project (TJoin)

option(ENABLE_TESTS "Enable test execution." OFF)

# Setup testing
enable_testing()
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(include)

set(MainCode
    src/Main.cpp
    src/Dijkstra.cpp
    src/TJoinShortestPath.cpp
)

add_executable(MainCode ${MainCode})

set(UnitTests
    tests/BinaryHeapTests.cpp
    tests/DijkstraTests.cpp
    tests/TJoinShortestPathTests.cpp
    src/Dijkstra.cpp
    src/TJoinShortestPath.cpp
)

if (ENABLE_TESTS)
    add_executable(UnitTests ${UnitTests})
    target_link_libraries(UnitTests
        ${GTEST_BOTH_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
    )

    add_custom_command(
         TARGET UnitTests
         COMMENT "Running tests. . ."
         POST_BUILD 
         COMMAND UnitTests
    )
endif()
