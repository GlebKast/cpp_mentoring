cmake_minimum_required(VERSION 3.25.0)
project(CopyTool VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

find_package(Boost 1.85.0 REQUIRED COMPONENTS system filesystem)

add_subdirectory(src)

target_link_libraries(CopyTool Boost::system Boost::filesystem)

if (BUILD_TESTING) 
    find_package(GTest) 
    if (GTEST_FOUND) 
        set(Gtest_FOUND TRUE) 
    endif () 
    if (GTest_FOUND) 
        include(GoogleTest) 
    else () 
        include(FetchContent) 
 
        FetchContent_Declare( 
                googletest 
                GIT_REPOSITORY https://github.com/google/googletest.git 
                GIT_TAG release-1.11.0 
                GIT_SHALLOW TRUE 
                GIT_PROGRESS TRUE) 
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) 
        FetchContent_MakeAvailable(googletest) 
 
    endif () 
 
    enable_testing()
    add_subdirectory(tests)
endif()

set_property(TARGET CopyTool PROPERTY CXX_STANDARD 17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
