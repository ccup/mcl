set(TEST_TARGET ${PROJECT_NAME}_test)

CPMAddPackage(
    NAME cctest
    GITHUB_REPOSITORY ccup/cctest
    GIT_TAG v1.0
)

file(GLOB_RECURSE SOURCES 
  "*.c" "*.C" "*.cc" "*.CC" "*.cpp" "*.CPP" "*.c++")

add_executable(${TEST_TARGET} ${SOURCES})

target_include_directories(${TEST_TARGET} PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TEST_TARGET} PRIVATE mcl cctest_main)

target_compile_features(${TEST_TARGET} PRIVATE cxx_std_17)

include(CTest)
enable_testing()
add_test(NAME test COMMAND ${TEST_TARGET})