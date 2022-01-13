add_subdirectory(domain)
add_subdirectory(service)

add_executable(mcl_example main.c)

target_include_directories(mcl_example 
    PRIVATE  ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(mcl_example PRIVATE mcl_example_service mcl pthread)
