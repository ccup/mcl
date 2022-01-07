file(GLOB_RECURSE SOURCES "*.c" "*.C")

add_library(mcl_example_service STATIC ${SOURCES})

target_include_directories(mcl_example_service 
    PUBLIC  ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src/service
)

target_link_libraries(mcl_example_service PRIVATE mcl_example_domain)
