cmake_minimum_required (VERSION 3.0)
project (TJoin)

option(ENABLE_TESTS "Enable test execution." OFF)

# Setup testing
enable_testing()
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(include)

set(BinaryHeapTests
    tests/BinaryHeapTests.cpp
)

set(DijkstraTests
    tests/DijkstraTests.cpp
    src/Dijkstra.cpp
)

set(TestJoinShortestPathTests
    tests/TJoinShortestPathTests.cpp
    src/TJoinShortestPath.cpp
    src/Dijkstra.cpp
)

set(UnitTests
    BinaryHeapTests
    DijkstraTests
    TestJoinShortestPathTests
)

foreach(Testname ${UnitTests})
    add_executable(${Testname} "${${Testname}}")
    target_link_libraries(${Testname}
        ${GTEST_BOTH_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
    )
endforeach(Testname ${UnitTests})

if (ENABLE_TESTS)
    foreach(UNIT_TEST ${UnitTests})
        add_custom_command(
             TARGET ${UNIT_TEST}
             COMMENT "Run test."
             POST_BUILD 
             COMMAND ${UNIT_TEST}
        )
    endforeach(UNIT_TEST ${UnitTests})
endif()
