cmake_minimum_required(VERSION 3.10)
project(ki017)

set(SOURCE
        main.cpp
        AI.cpp
        AIController.cpp
        util/Logging.cpp
        generate/ItemChoice.cpp
        generate/EquipmentChoice.cpp
        generate/GameOperation.cpp
        generate/random/ItemChoice.cpp
        generate/random/EquipmentChoice.cpp
        generate/random/GameOperation.cpp
        )

add_executable(${PROJECT_NAME} ${SOURCE})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMMON_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -mtune=native -march=native)
target_link_libraries(${PROJECT_NAME}
        spdlog::spdlog
        CLI11::CLI11 stdc++fs
        SopraNetwork
        SopraCommon
        SopraClient
        $<$<CONFIG:Debug>:--coverage>)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)


target_link_libraries(${PROJECT_NAME} ${LIBS})
