set(TEST_TARGET ${PROJECT_NAME}_test)

CPMAddPackage(
    NAME cctest
    GITHUB_REPOSITORY ccup/cctest
    GIT_TAG v1.0
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
  "*.c" "*.C" "*.cc" "*.CC" "*.cpp" "*.CPP" "*.c++")
  
if (ENABLE_THREAD)
    message(STATUS "Thread tests enabled!")
else()
    list(APPEND TASK_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/lock/atom_test.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/lock/future_test.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/lock/lockobj_test.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/lock/lockptr_test.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/lock/mutex_test.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/mem/shared_ptr_thread_test.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/msg/msg_queue_thread_test.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/ringbuff/ringbuff_thread_test.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/task/task_scheduler_test.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/thread/thread_launcher_test.cpp
    )
    list(REMOVE_ITEM SOURCES ${TASK_SOURCES})
    message(STATUS "Thread tests disabled, removed tests in sources : ${TASK_SOURCES}!")
endif()

add_executable(${TEST_TARGET} ${SOURCES})

target_include_directories(${TEST_TARGET} PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TEST_TARGET} PRIVATE mcl cctest_main)

target_compile_features(${TEST_TARGET} PRIVATE cxx_std_17)

target_compile_options(${TEST_TARGET} 
    PRIVATE -Wall -Wextra -pedantic -Werror 
    -Wno-gnu-zero-variadic-macro-arguments 
    -Wno-unused-parameter 
    -Wno-unused-variable 
    -Wno-c99-extensions 
    -Wno-gnu-statement-expression
    -Wno-missing-field-initializers
    -Wno-language-extension-token)

include(CTest)
enable_testing()
add_test(NAME test COMMAND ${TEST_TARGET})