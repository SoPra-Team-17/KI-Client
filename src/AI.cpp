//
// Created by Carolin on 16.05.2020.
//

#include "AI.hpp"

#include <generate/ItemChoice.hpp>
#include <generate/EquipmentChoice.hpp>
#include <generate/GameOperation.hpp>
#include "util/Logging.hpp"

AI::AI(std::string address, uint16_t port, std::string name, unsigned int verbosity,
       unsigned int difficulty,
       std::map<std::string, std::string> additionalOptions) : address(std::move(address)), port(port),
                                                               name(std::move(name)),
                                                               difficulty(difficulty), libClientHandler{this} {

    maxReconnect = additionalOptions.find("maxReconnect") != additionalOptions.end() ? std::stoi(
            additionalOptions.at("maxReconnect")) : 5;

    // set up logging
    Logging::initLogging(verbosity);

    connect();
}

void AI::connect() {
    // connect to server as AI
    if (!libClientHandler.network.connect(address, port)) {
        spdlog::critical("could not connect to server");
        exit(2);
    }
    spdlog::info("connected to server");
    if (!libClientHandler.network.sendHello(name, spy::network::RoleEnum::AI)) {
        spdlog::critical("could not send Hello message");
        exit(2);
    }
    spdlog::info("send Hello message");

    // TODO make sure that matchConfig is available
}

void AI::onHelloReply() {
    spdlog::info("received HelloReply message");
    // do nothing
}

void AI::onGameStarted() {
    spdlog::info("received GameStarted message");
    // do nothing
}

void AI::onRequestItemChoice() {
    spdlog::info("received RequestItemChoice message");

    auto choice = ItemChoice::generate(difficulty, libClientHandler.getOfferedCharacters(),
                                       libClientHandler.getOfferedGadgets(), matchConfig);
    if (!libClientHandler.network.sendItemChoice(choice)) {
        spdlog::critical("could not send ItemChoice message");
        exit(2);
    }
    spdlog::info("sent ItemChoice message");
}

void AI::onRequestEquipmentChoice() {
    spdlog::info("received RequestEquipmentChoice message");

    auto equipment = EquipmentChoice::generate(difficulty, libClientHandler.getChosenCharacters(),
                                               libClientHandler.getChosenGadgets(), matchConfig);
    if (!libClientHandler.network.sendEquipmentChoice(equipment)) {
        spdlog::critical("could not send EquipmentChoice message");
        exit(2);
    }
    spdlog::info("sent EquipmentChoice message");
}

void AI::onGameStatus() {
    spdlog::info("received GameStatus message");

    // TODO save additional info (can this be done in LibClient to help Client too) ?
}

void AI::onRequestGameOperation() {
    spdlog::info("received RequestGameOperation message");

    auto operation = GameOperation::generate(difficulty, libClientHandler.getActiveCharacter(),
                                             libClientHandler.getState(), matchConfig);
    if (!libClientHandler.network.sendGameOperation(operation, matchConfig)) {
        spdlog::critical("could not send GameOperation message");
        exit(2);
    }
    spdlog::info("sent GameOperation message");
}

void AI::onStatistics() {
    spdlog::info("received Statistics message");

    // TODO use as additional info ?
    std::string endString = "Game ";
    endString += libClientHandler.getWinner().value() == libClientHandler.getId().value() ? "won!" : "lost!";
    spdlog::info(endString);
    libClientHandler.network.disconnect();
    exit(0);
}

void AI::onGameLeft() {
    spdlog::info("received GameLeft message");
    // do nothing (next message is Statistics message)
}

void AI::onGamePause() {
    spdlog::info("received GamePause message");
    // do nothing
}

void AI::onMetaInformation() {
    spdlog::info("received MetaInformation message");

    // TODO what to do with received information ?
    // TODO MatchConfig is needed !!!
}

void AI::onStrike() {
    spdlog::info("received Strike message");

    spdlog::warn("ki caused a strike");
}

void AI::onError() {
    spdlog::info("received Error message");

    switch (libClientHandler.getErrorReason().value()) {
        case spy::network::ErrorTypeEnum::NAME_NOT_AVAILABLE:
            spdlog::error("error type is name not available");
            if (name == "ki017") {
                spdlog::critical("default name is not available");
                exit(1);
            }
            name = "ki017";
            connect();
            break;
        case spy::network::ErrorTypeEnum::ALREADY_SERVING:
            spdlog::critical("error type is already serving");
            exit(1);
        case spy::network::ErrorTypeEnum::SESSION_DOES_NOT_EXIST:
            spdlog::critical("error type is session does not exist");
            exit(1);
        case spy::network::ErrorTypeEnum::ILLEGAL_MESSAGE:
            spdlog::critical("error type is illegal message");
            exit(1);
        case spy::network::ErrorTypeEnum::TOO_MANY_STRIKES:
            spdlog::critical("error type is too many strikes");
            exit(1);
        case spy::network::ErrorTypeEnum::GENERAL:
            [[fallthrough]];
        default:
            spdlog::critical("error type is general or unknown");
            exit(1);
    }
}

void AI::onReplay() {
    spdlog::info("received Replay message");
}

void AI::connectionLost() {
    spdlog::info("received connection lost callback");

    for (unsigned int i = 0; i < maxReconnect; i++) {
        if (libClientHandler.network.sendReconnect()) {
            spdlog::info("sent Reconnect message");
            return;
        }
    }
    spdlog::critical("could not reconnect");
    exit(2);
}

void AI::wrongDestination() {
    spdlog::debug("received message that was not meant for me");
    // do nothing
}


