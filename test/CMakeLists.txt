set(TEST_TARGET ${PROJECT_NAME}_test)

CPMAddPackage(
    NAME cctest
    GITHUB_REPOSITORY ccup/cctest
    GIT_TAG v1.0
)

file(GLOB_RECURSE SOURCES 
  "*.c" "*.C" "*.cc" "*.CC" "*.cpp" "*.CPP" "*.c++")

if (ENABLE_THREAD)
    message(STATUS "Thread tests enabled!")
else()
    message(STATUS "Thread tests disabled!")
    list(REMOVE_ITEM SOURCES
            "${CMAKE_CURRENT_SOURCE_DIR}/lock_test.cc"
            "${CMAKE_CURRENT_SOURCE_DIR}/mutex_test.cc"
            "${CMAKE_CURRENT_SOURCE_DIR}/scheduler_test.cc"
    )
endif()

add_executable(${TEST_TARGET} ${SOURCES})

target_include_directories(${TEST_TARGET} PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TEST_TARGET} PRIVATE mcl cctest_main)

target_compile_features(${TEST_TARGET} PRIVATE cxx_std_17)

include(CTest)
enable_testing()
add_test(NAME test COMMAND ${TEST_TARGET})