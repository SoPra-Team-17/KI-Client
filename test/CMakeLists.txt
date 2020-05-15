cmake_minimum_required(VERSION 3.10)

include_directories(../src)

set(SOURCE
        test1.cpp
        )

add_executable(KITests ${SOURCE})
target_link_libraries(KITests
		gtest
		gmock
		gtest_main
		SopraCommon
		SopraClient
		$<$<CONFIG:Debug>:--coverage>)
gtest_discover_tests(KITests)