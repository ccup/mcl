cmake_minimum_required(VERSION 3.4)

project(mcl)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        ${PROJECT_SOURCE_DIR}/include
)

if(ENABLE_TEST)
    add_subdirectory(test)
endif()

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ 
    DESTINATION include)