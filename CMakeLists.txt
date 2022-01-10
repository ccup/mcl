cmake_minimum_required(VERSION 3.14)

project(mcl)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

option(ENABLE_CUSTOMIZED   "Enable Cumtomize" OFF)
option(ENABLE_THREAD       "Enable thread"    ON)
option(ENABLE_EXAMPLE      "Build example"    ON)
option(ENABLE_TEST         "Build tests"      ON)
option(ENABLE_ASON         "Enable AddressSanitizer" ON)
option(ENABLE_TSAN         "Enable ThreadSanitizer"  OFF)

include(${PROJECT_SOURCE_DIR}/cmake/ENV.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)

if(ENABLE_CUSTOMIZED)
    message(STATUS "Customized config enabled")
    add_definitions("-DMCL_CONFIG")
endif()

if(ENABLE_THREAD)
    message(STATUS "Thread enabled")
    add_definitions("-DMCL_THREAD_ENABLED")
endif()

if(ENABLE_ASON)
    message(STATUS "AddressSanitizer enabled")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address") 
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address") 
endif()

if(ENABLE_TSAN)
    message(STATUS "ThreadSanitizer enabled")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O1 -fsanitize=thread -fno-omit-frame-pointer -fPIC -fPIE")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 -fsanitize=thread -fno-omit-frame-pointer -fPIC -fPIE")
endif()

add_subdirectory(src)

if(ENABLE_EXAMPLE)
    add_subdirectory(example)
endif()

if(ENABLE_TEST)
    add_subdirectory(test)
endif()

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ 
    DESTINATION include)