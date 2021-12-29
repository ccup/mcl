set(TEST_TARGET ${PROJECT_NAME}_test)

CPMAddPackage(
    NAME cctest
    GITHUB_REPOSITORY ccup/cctest
    GIT_TAG v1.0
)

file(GLOB_RECURSE SOURCES 
  "*.c" "*.C" "*.cc" "*.CC" "*.cpp" "*.CPP" "*.c++")
  
file(GLOB_RECURSE TASK_SOURCES "task/*_test.cc")
  
if (ENABLE_THREAD)
    message(STATUS "Thread tests enabled!")
else()
    list(REMOVE_ITEM TASK_SOURCES 
        ${CMAKE_CURRENT_SOURCE_DIR}/task/ringbuff_test.cc
#        ${CMAKE_CURRENT_SOURCE_DIR}/task/lockobj_test.cc
#        ${CMAKE_CURRENT_SOURCE_DIR}/task/lockptr_test.cc
#        ${CMAKE_CURRENT_SOURCE_DIR}/task/atom_test.cc
#        ${CMAKE_CURRENT_SOURCE_DIR}/task/shared_ptr_thread_test.cc
    )
    list(REMOVE_ITEM SOURCES ${TASK_SOURCES})
    message(STATUS "Thread tests disabled, removed tests in sources : ${TASK_SOURCES}!")
endif()

add_executable(${TEST_TARGET} ${SOURCES})

target_include_directories(${TEST_TARGET} PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TEST_TARGET} PRIVATE mcl cctest_main)

target_compile_features(${TEST_TARGET} PRIVATE cxx_std_17)

include(CTest)
enable_testing()
add_test(NAME test COMMAND ${TEST_TARGET})