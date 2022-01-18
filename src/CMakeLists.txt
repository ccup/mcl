file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.c" "*.C")

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror -Wno-gnu-zero-variadic-macro-arguments -Wno-unused-parameter -Wno-unused-variable)

if(ENABLE_THREAD)
    target_link_libraries(${PROJECT_NAME} PUBLIC pthread)
endif()

