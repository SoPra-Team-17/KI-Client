//
// Created by Carolin on 16.05.2020.
//

#include "AIController.hpp"

#include <utility>


AIController::AIController(AI &ai) : ai(ai) {}

void AIController::onHelloReply() {
    spdlog::info("received HelloReply message");
    // do nothing
}

void AIController::onGameStarted() {
    spdlog::info("received GameStarted message");
    // do nothing
}

void AIController::onRequestItemChoice() {
    spdlog::info("received RequestItemChoice message");
    ai.itemChoice();
}

void AIController::onRequestEquipmentChoice() {
    spdlog::info("received RequestEquipmentChoice message");
    ai.equipmentChoice();
}

void AIController::onGameStatus() {
    spdlog::info("received GameStatus message");
    ai.gameStatus();
}

void AIController::onRequestGameOperation() {
    spdlog::info("received RequestGameOperation message");
    ai.gameOperation();
}

void AIController::onStatistics() {
    spdlog::info("received Statistics message");
    ai.statistics();
}

void AIController::onGameLeft() {
    spdlog::info("received GameLeft message");
    // do nothing (next message is Statistics message)
}

void AIController::onGamePause() {
    spdlog::info("received GamePause message");
    // do nothing
}

void AIController::onMetaInformation() {
    spdlog::info("received MetaInformation message");
    ai.metaInformation();
}

void AIController::onStrike() {
    spdlog::info("received Strike message");
    ai.strike();
}

void AIController::onError() {
    spdlog::info("received Error message");
    ai.error();
}

void AIController::onReplay() {
    spdlog::info("received Replay message");
}

void AIController::connectionLost() {
    spdlog::info("received connection lost callback");
    ai.connectionLost();
}

void AIController::wrongDestination() {
    spdlog::debug("received message that was not meant for me");
    // do nothing
}

AIController::AIController(const std::string &address,
                           uint16_t port,
                           const std::string &name,
                           unsigned int verbosity,
                           unsigned int difficulty,
                           std::map<std::string, std::string> additionalOptions) :
        libClientHandler{this},
        ai{address, port, name, verbosity, difficulty, std::move(additionalOptions)} {}
