cmake_minimum_required(VERSION 3.25.0)
project(mentoringProj VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(mentoringProj main.cpp)

find_package(GTest)
if (GTest_FOUND)
    project(mentoringProj_tests CXX)

    set(GTEST_FILES
        unit_tests/GTestMain.cpp
        unit_tests/multithreadingUnitTests.cpp
    )

    include_directories(${GTEST_INCLUDE_DIRS})
    add_executable(${PROJECT_NAME} ${SRC_FILES_EXCEPT_MAIN} ${GTEST_FILES})
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})
    target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES})

    set_property(TARGET mentoringProj_tests PROPERTY CXX_STANDARD 17)
endif()

set_property(TARGET mentoringProj PROPERTY CXX_STANDARD 17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
