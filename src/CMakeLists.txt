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
        generate/utils/execute/OperationExecutor_Movement.cpp
        generate/utils/execute/OperationExecutor_Retire.cpp
        generate/utils/execute/OperationExecutor_Gamble.cpp
        generate/utils/execute/OperationExecutor_Gadget.cpp
        generate/utils/execute/OperationExecutor_Property.cpp
        generate/utils/execute/OperationExecutor_Spy.cpp
        generate/utils/execute/property/OperationExecutor_BangAndBurn.cpp
        generate/utils/execute/property/OperationExecutor_Observation.cpp
        generate/utils/execute/gadget/OperationExecutor_BowlerBlade.cpp
        generate/utils/execute/gadget/OperationExecutor_ChickenFeed.cpp
        generate/utils/execute/gadget/OperationExecutor_Cocktail.cpp
        generate/utils/execute/gadget/OperationExecutor_DiamondCollar.cpp
        generate/utils/execute/gadget/OperationExecutor_FogTin.cpp
        generate/utils/execute/gadget/OperationExecutor_GasGloss.cpp
        generate/utils/execute/gadget/OperationExecutor_Grapple.cpp
        generate/utils/execute/gadget/OperationExecutor_HairDryer.cpp
        generate/utils/execute/gadget/OperationExecutor_Jetpack.cpp
        generate/utils/execute/gadget/OperationExecutor_LaserCompact.cpp
        generate/utils/execute/gadget/OperationExecutor_LaserCompact.cpp
        generate/utils/execute/gadget/OperationExecutor_MirrorOfWilderness.cpp
        generate/utils/execute/gadget/OperationExecutor_MoleDie.cpp
        generate/utils/execute/gadget/OperationExecutor_MothballPouch.cpp
        generate/utils/execute/gadget/OperationExecutor_Nugget.cpp
        generate/utils/execute/gadget/OperationExecutor_PoisonPills.cpp
        generate/utils/execute/gadget/OperationExecutor_RocketPen.cpp
        generate/utils/execute/gadget/OperationExecutor_TechnicolorPrism.cpp
        generate/utils/execute/gadget/OperationExecutor_WiretapWithEarplugs.cpp
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