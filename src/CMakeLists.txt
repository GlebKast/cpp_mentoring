add_executable(CopyTool 
    main.cpp
    CopyToolFactory.cpp
    multithreading/MultithreadingChunkController.cpp
    multithreading/MultithreadingCopyTool.cpp 
    multithreading/MultithreadingCopyToolFactory.cpp 
    multithreading/MultithreadingCopyToolValidator.cpp
    multiprocessing/MultiprocessingChunkController.cpp
    multiprocessing/MultiprocessingCopyTool.cpp
    multiprocessing/MultiprocessingCopyToolFactory.cpp
    multiprocessing/MultiprocessingCopyToolValidator.cpp
    fileManager/stream/StreamFileReader.cpp
    fileManager/stream/StreamFileWriter.cpp
    utils/Utils.cpp)

target_include_directories(CopyTool PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/utils
    ${CMAKE_SOURCE_DIR}/include/multiprocessing
    ${CMAKE_SOURCE_DIR}/include/multithreading
    ${CMAKE_SOURCE_DIR}/include/fileManager/stream
    ${CMAKE_SOURCE_DIR}/include/fileManager
)

set_target_properties(CopyTool PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)