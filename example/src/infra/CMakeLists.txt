file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.c" "*.C")

add_library(mcl_example_infra STATIC ${SOURCES})

target_include_directories(mcl_example_infra 
    PUBLIC  ${PROJECT_SOURCE_DIR}/include
    PUBLIC  ${PROJECT_SOURCE_DIR}/src/infra
)

target_link_libraries(mcl_example_infra PUBLIC mcl)
