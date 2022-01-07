file(GLOB_RECURSE SOURCES "*.c" "*.C")

add_library(mcl_example_domain STATIC ${SOURCES})

target_include_directories(mcl_example_domain 
    PUBLIC  ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src/domain
)

target_link_libraries(mcl_example_domain PUBLIC mcl)
