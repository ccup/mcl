# ---- OS detect ----
message(STATUS "mcl os: ${CMAKE_SYSTEM}")

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    add_definitions("-DMCL_OS_LINUX")

elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    add_definitions("-DMCL_OS_MAC")

elseif (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
    add_definitions("-DMCL_OS_FREE_BSD")

elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    add_definitions("-DMCL_OS_WINDOWS")   

else ()
    message(FATAL_ERROR "Unsuport os platform: ${CMAKE_SYSTEM_NAME}")

endif()

# ---- Compiler detect ----
message(STATUS "mcl compiler: ${CMAKE_CXX_COMPILER_ID}")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    add_definitions("-DMCL_COMPILER_APPLE_CLANG")
    set(WHOLE_ARCHIVE -all_load)
    set(NO_WHOLE_ARCHIVE -noall_load)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_definitions("-DMCL_COMPILER_GNU")
    set(WHOLE_ARCHIVE --whole-archive)
    set(NO_WHOLE_ARCHIVE --no-whole-archive)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions("-DMCL_COMPILER_CLANG")
    set(WHOLE_ARCHIVE -all_load)
    set(NO_WHOLE_ARCHIVE -noall_load)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    add_definitions("-DMCL_COMPILER_INTEL")
    message(FATAL_ERROR "Compiler ${CMAKE_CXX_COMPILER_ID} unsuported! ")

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_definitions("-DMCL_COMPILER_MSVC")
    message(FATAL_ERROR "Compiler ${CMAKE_CXX_COMPILER_ID} unsuported! ")

else()
    message(FATAL_ERROR "Compiler ${CMAKE_CXX_COMPILER_ID} unsuported! ")

endif()