cmake_minimum_required(VERSION 3.25.0)
project(mentoringProj VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(mentoringProj main.cpp)

set_property(TARGET mentoringProj PROPERTY CXX_STANDARD 17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
