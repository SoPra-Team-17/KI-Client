//
// Created by Carolin on 16.05.2020.
//

#include "AI.hpp"

#include <generate/ItemChoice.hpp>
#include <generate/EquipmentChoice.hpp>
#include <generate/GameOperation.hpp>
#include "util/Logging.hpp"
#include "AICallback.hpp"

AI::AI(const std::string &address, uint16_t port, const std::string &name, unsigned int verbosity,
       unsigned int difficulty,
       std::map<std::string, std::string> additionalOptions) : address(address), port(port), name(name),
                                                               difficulty(difficulty), libClientHandler{std::dynamic_pointer_cast<libclient::Callback>(std::make_shared<AICallback>(*this))} {

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

void AI::itemChoice() {
    auto choice = ItemChoice::generate(difficulty, libClientHandler.getOfferedCharacters(),
                                       libClientHandler.getOfferedGadgets(), matchConfig);
    if (!libClientHandler.network.sendItemChoice(choice)) {
        spdlog::critical("could not send ItemChoice message");
        exit(2);
    }
    spdlog::info("sent ItemChoice message");
}

void AI::equipmentChoice() {
    auto equipment = EquipmentChoice::generate(difficulty, libClientHandler.getChosenCharacters(),
                                               libClientHandler.getChosenGadgets(), matchConfig);
    if (!libClientHandler.network.sendEquipmentChoice(equipment)) {
        spdlog::critical("could not send EquipmentChoice message");
        exit(2);
    }
    spdlog::info("sent EquipmentChoice message");
}

void AI::gameStatus() {
    // TODO save additional info (can this be done in LibClient to help Client too) ?
}

void AI::gameOperation() {
    auto operation = GameOperation::generate(difficulty, libClientHandler.getActiveCharacter(),
                                             libClientHandler.getState(), matchConfig);
    if (!libClientHandler.network.sendGameOperation(operation, matchConfig)) {
        spdlog::critical("could not send GameOperation message");
        exit(2);
    }
    spdlog::info("sent GameOperation message");
}

void AI::statistics() {
    // TODO use as additional info ?
    std::string endString = "Game ";
    endString += libClientHandler.getWinner().value() == libClientHandler.getId().value() ? "won!" : "lost!";
    spdlog::info(endString);
    libClientHandler.network.disconnect();
    exit(0);
}

void AI::metaInformation() {
    // TODO what to do with received information ?
    // TODO MatchConfig is needed !!!
}

void AI::strike() {
    spdlog::warn("ki caused a strike");
}

void AI::error() {
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

void AI::connectionLost() {
    for (unsigned int i = 0; i < maxReconnect; i++) {
        if (libClientHandler.network.sendReconnect()) {
            spdlog::info("sent Reconnect message");
            return;
        }
    }
    spdlog::critical("could not reconnect");
    exit(2);
}


