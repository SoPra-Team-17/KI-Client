cmake_minimum_required(VERSION 3.10)
project(KIClient)

# build options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -mtune=native -march=native")

set(SOURCE
		main.cpp
	)
	
include_directories(.)

add_executable(${PROJECT_NAME} ${SOURCE})