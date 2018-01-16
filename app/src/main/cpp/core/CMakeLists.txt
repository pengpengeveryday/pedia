ADD_LIBRARY(core-lib SHARED
    Lock.cpp
    MessageQueue.cpp
    Semaphore.cpp
    SingleThread.cpp)

target_link_libraries(core-lib log)