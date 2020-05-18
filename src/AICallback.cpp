//
// Created by Carolin on 16.05.2020.
//

#include "AICallback.hpp"


AICallback::AICallback(AI &ai) : ai(ai) {}

void AICallback::onHelloReply() {
    spdlog::info("received HelloReply message");
    // do nothing
}

void AICallback::onGameStarted() {
    spdlog::info("received GameStarted message");
    // do nothing
}

void AICallback::onRequestItemChoice() {
    spdlog::info("received RequestItemChoice message");
    ai.itemChoice();
}

void AICallback::onRequestEquipmentChoice() {
    spdlog::info("received RequestEquipmentChoice message");
    ai.equipmentChoice();
}

void AICallback::onGameStatus() {
    spdlog::info("received GameStatus message");
    ai.gameStatus();
}

void AICallback::onRequestGameOperation() {
    spdlog::info("received RequestGameOperation message");
    ai.gameOperation();
}

void AICallback::onStatistics() {
    spdlog::info("received Statistics message");
    ai.statistics();
}

void AICallback::onGameLeft() {
    spdlog::info("received GameLeft message");
    // do nothing (next message is Statistics message)
}

void AICallback::onGamePause() {
    spdlog::info("received GamePause message");
    // do nothing
}

void AICallback::onMetaInformation() {
    spdlog::info("received MetaInformation message");
    ai.metaInformation();
}

void AICallback::onStrike() {
    spdlog::info("received Strike message");
    ai.strike();
}

void AICallback::onError() {
    spdlog::info("received Error message");
    ai.error();
}

void AICallback::onReplay() {
    spdlog::info("received Replay message");
}

void AICallback::connectionLost() {
    spdlog::info("received connection lost callback");
    ai.connectionLost();
}

void AICallback::wrongDestination() {
    spdlog::debug("received message that was not meant for me");
    // do nothing
}
