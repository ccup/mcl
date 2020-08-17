project(test_mcl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-offsetof -g -std=gnu++11")

set (log_output_func_declare "void mcl_color_log(int level , const char* levelstr , const char* file, unsigned int line, const char* fmt, ...)")
set (log_output_func "mcl_color_log")
set (log_enabled_levels "MCL_TOTAL_LEVEL")

configure_file (
    "${CMAKE_HOME_DIRECTORY}/include/mcl/log_customized_config.h.in"
    "${CMAKE_HOME_DIRECTORY}/include/mcl/log_customized_config.h"
)

file(GLOB_RECURSE SOURCES
*.cpp
*.cc
*.c++
*.c
*.C)

link_directories("/usr/local/lib")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR} "/usr/local/include")

target_link_libraries(${PROJECT_NAME} PRIVATE mcl cctest cctest_main)

target_compile_definitions(${PROJECT_NAME} PRIVATE CUSTOMER_LOG)