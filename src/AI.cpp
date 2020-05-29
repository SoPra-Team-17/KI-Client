//
// Created by Carolin on 16.05.2020.
//

#include "AI.hpp"

#include <generate/ItemChoice_gen.hpp>
#include <generate/EquipmentChoice_gen.hpp>
#include <generate/GameOperation_gen.hpp>
#include <utility>
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
        std::exit(2);
    }
    spdlog::info("connected to server");
    if (!libClientHandler.network.sendHello(name, spy::network::RoleEnum::AI)) {
        spdlog::critical("could not send Hello message");
        std::exit(2);
    }
    spdlog::info("send Hello message");
}

void AI::requestConfigs() {
    using namespace spy::network::messages;
    if (!libClientHandler.network.sendRequestMetaInformation(
            {MetaInformationKey::CONFIGURATION_MATCH_CONFIG, MetaInformationKey::CONFIGURATION_SCENARIO,
             MetaInformationKey::CONFIGURATION_CHARACTER_INFORMATION})) {
        spdlog::critical("could not send RequestMetaInformation message for configs");
        exit(2);
    }
    spdlog::info("send RequestMetaInformation message for configs");
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

    if (!matchConfig.has_value() || !scenarioConfig.has_value() || !characterConfig.has_value()) {
        spdlog::error("configs for item choice are not available");
        configsWereNotAvailable = true;
        requestConfigs();
        return;
    }
    auto choice = ItemChoice_gen::generate(difficulty, libClientHandler.getOfferedCharacters(),
                                           libClientHandler.getOfferedGadgets(), matchConfig.value(),
                                           scenarioConfig.value(),
                                           characterConfig.value());
    if (!libClientHandler.network.sendItemChoice(choice)) {
        spdlog::critical("could not send ItemChoice_gen message");
        std::exit(2);
    }
    spdlog::info("sent ItemChoice message");
}

void AI::onRequestEquipmentChoice() {
    spdlog::info("received RequestEquipmentChoice message");

    auto equipment = EquipmentChoice_gen::generate(difficulty, libClientHandler.getChosenCharacters(),
                                                   libClientHandler.getChosenGadgets(), matchConfig.value(),
                                                   scenarioConfig.value(),
                                                   characterConfig.value());
    if (!libClientHandler.network.sendEquipmentChoice(equipment)) {
        spdlog::critical("could not send EquipmentChoice_gen message");
        std::exit(2);
    }
    spdlog::info("sent EquipmentChoice message");
}

void AI::onGameStatus() {
    spdlog::info("received GameStatus message");

    // TODO save additional info (can this be done in LibClient to help Client too) ?
}

void AI::onRequestGameOperation() {
    spdlog::info("received RequestGameOperation message");

    auto operation = GameOperation_gen::generate(difficulty, libClientHandler.getActiveCharacter(),
                                                 libClientHandler.getState(), matchConfig.value());
    if (!libClientHandler.network.sendGameOperation(operation, matchConfig.value())) {
        spdlog::critical("could not send GameOperation_gen message");
        std::exit(2);
    }
    spdlog::info("sent GameOperation message");
}

void AI::onStatistics() {
    spdlog::info("received Statistics message");

    // TODO use as additional info ?
    std::string endString = "Game ";
    endString += libClientHandler.getWinner().value() == libClientHandler.getId().value() ? "won!" : "lost!";
    spdlog::info(endString);
    std::exit(0);
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

    using namespace spy::network::messages;
    auto infoMap = libClientHandler.getInformation();
    matchConfig = std::get<spy::MatchConfig>(infoMap.at(MetaInformationKey::CONFIGURATION_MATCH_CONFIG));
    scenarioConfig = std::get<spy::scenario::Scenario>(infoMap.at(MetaInformationKey::CONFIGURATION_SCENARIO));
    characterConfig = std::get<std::vector<spy::character::CharacterInformation>>(infoMap.at(MetaInformationKey::CONFIGURATION_MATCH_CONFIG));

    if (configsWereNotAvailable) {
        configsWereNotAvailable = false;
        onRequestItemChoice();
    }
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
                std::exit(1);
            }
            name = "ki017";
            connect();
            break;
        case spy::network::ErrorTypeEnum::ALREADY_SERVING:
            spdlog::critical("error type is already serving");
            std::exit(1);
        case spy::network::ErrorTypeEnum::SESSION_DOES_NOT_EXIST:
            spdlog::critical("error type is session does not exist");
            std::exit(1);
        case spy::network::ErrorTypeEnum::ILLEGAL_MESSAGE:
            spdlog::critical("error type is illegal message");
            std::exit(1);
        case spy::network::ErrorTypeEnum::TOO_MANY_STRIKES:
            spdlog::critical("error type is too many strikes");
            std::exit(1);
        case spy::network::ErrorTypeEnum::GENERAL:
            [[fallthrough]];
        default:
            spdlog::critical("error type is general or unknown");
            std::exit(1);
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
    std::exit(2);
}

void AI::wrongDestination() {
    spdlog::debug("received message that was not meant for me");
    // do nothing
}


