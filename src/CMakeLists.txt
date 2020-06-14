cmake_minimum_required(VERSION 3.10)
project(ki017)

set(SOURCE
        main.cpp
        AI.cpp
        util/Logging.cpp
        generate/ItemChoice_gen.cpp
        generate/EquipmentChoice_gen.cpp
        generate/GameOperation_gen.cpp
        generate/random/ItemChoice_random.cpp
        generate/random/EquipmentChoice_random.cpp
        generate/random/GameOperation_random.cpp
        generate/caro/ItemChoice_caro.cpp
        generate/caro/EquipmentChoice_caro.cpp
        generate/caro/GameOperation_caro.cpp
        generate/caro/evalFunctions_caro.cpp
        generate/utils/State_AI.cpp
        generate/utils/generate/OperationGenerator.cpp
        generate/utils/execute/OperationExecutor.cpp
        )

add_executable(${PROJECT_NAME} ${SOURCE})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMMON_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -mtune=native -march=native)
target_link_libraries(${PROJECT_NAME}
        spdlog::spdlog
        CLI11::CLI11 stdc++fs
        SopraCommon
        SopraClient
        $<$<CONFIG:Debug>:--coverage>)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)


add_library(SopraAI SHARED ${SOURCE})
target_compile_options(SopraAI PRIVATE ${COMMON_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -mtune=native -march=native)
target_link_libraries(SopraAI
        SopraCommon
        SopraClient
        $<$<CONFIG:Debug>:--coverage>)
target_include_directories(SopraAI
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_compile_features(SopraAI PRIVATE cxx_std_17)

include(GNUInstallDirs)
install(TARGETS SopraAI DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/generate"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SopraAI
        FILES_MATCHING PATTERN "*.hpp"
        )