set(TEST_TARGET ${PROJECT_NAME}_test)

include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)

CPMAddPackage(
    NAME cctest
    GITHUB_REPOSITORY ccup/cctest
    GIT_TAG master
)

set (log_output_func_declare "void mcl_color_log(int level , const char* levelstr , const char* file, unsigned int line, const char* fmt, ...)")
set (log_output_func "mcl_color_log")
set (log_enabled_levels "MCL_TOTAL_LEVEL")

configure_file (
    "${PROJECT_SOURCE_DIR}/include/mcl/log_customized_config.h.in"
    "${PROJECT_SOURCE_DIR}/include/mcl/log_customized_config.h"
)

file(GLOB_RECURSE SOURCES 
  "*.c" "*.C" "*.cc" "*.CC" "*.cpp" "*.CPP" "*.c++")

add_executable(${TEST_TARGET} ${SOURCES})

target_include_directories(${TEST_TARGET} PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TEST_TARGET} PRIVATE mcl cctest_main)

set_target_properties(${TEST_TARGET} PROPERTIES CXX_STANDARD 14)
target_compile_definitions(${TEST_TARGET} PRIVATE CUSTOMER_LOG)