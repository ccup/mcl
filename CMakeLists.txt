cmake_minimum_required(VERSION 3.14)

project(mcl)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

option(ENABLE_TEST "Build tests" OFF)
option(ENABLE_ASON "Enable AddressSanitizer" OFF)
option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)

include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)

if(ENABLE_ASON)
    message(STATUS "AddressSanitizer enabled")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address") 
endif()

if(ENABLE_TSAN)
    message(STATUS "ThreadSanitizer enabled")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 -fsanitize=thread -fno-omit-frame-pointer -fPIC -fPIE")
endif()

add_subdirectory(src)

if(ENABLE_TEST)
    add_subdirectory(test)
endif()

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ 
    DESTINATION include)