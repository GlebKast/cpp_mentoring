add_executable(CopyTool 
    main.cpp 
    multithreading/MultithreadingCopyTool.cpp 
    multithreading/MultithreadingCopyToolFactory.cpp 
    multiprocessing/MultiprocessingCopyTool.cpp
    multiprocessing/MultiprocessingCopyToolFactory.cpp
    CopyToolFactory.cpp)

target_include_directories(CopyTool PUBLIC
    ${CMAKE_SOURCE_DIR}/include/multiprocessing
    ${CMAKE_SOURCE_DIR}/include/multithreading
    ${CMAKE_SOURCE_DIR}/include
)

set_target_properties(CopyTool PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)