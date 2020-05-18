cmake_minimum_required(VERSION 3.10)
project(KIClient)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif (NOT CMAKE_BUILD_TYPE)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("\n##########################")
    message("###### DEBUG BUILD #######")
    message("##########################\n")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
else ()
    message("\n############################")
    message("###### RELEASE BUILD #######")
    message("############################\n")
endif ()

find_package(spdlog REQUIRED)
find_package(CLI11 REQUIRED)
add_definitions(-DCLI11_HAS_FILESYSTEM=1)

add_subdirectory(src)
add_subdirectory(extern/LibClient)

option(KI_TESTS "Build the tests" ON)
if (KI_TESTS)
    if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
        # Download and unpack googletest at configure time
        configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
        execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                RESULT_VARIABLE result
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
        if (result)
            message(FATAL_ERROR "CMake step for googletest failed: ${result}")
        endif ()

        execute_process(COMMAND ${CMAKE_COMMAND} --build .
                RESULT_VARIABLE result
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
        if (result)
            message(FATAL_ERROR "Build step for googletest failed: ${result}")
        endif ()

        # Add googletest directly to the build
        add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
                "${CMAKE_BINARY_DIR}/googletest-build"
                )
    endif()
    enable_testing()
    include(GoogleTest)
    add_subdirectory(test)
endif ()