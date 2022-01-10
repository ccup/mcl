file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.c" "*.C")

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(ENABLE_THREAD)
    target_link_libraries(${PROJECT_NAME} PUBLIC pthread)
endif()

