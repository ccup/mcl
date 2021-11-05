cmake_minimum_required(VERSION 3.14)

project(mcl)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

option(ENABLE_TEST "Build tests" OFF)

include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)

add_subdirectory(src)

if(ENABLE_TEST)
    add_subdirectory(test)
endif()

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ 
    DESTINATION include)