//
// Created by Carolin on 16.05.2020.
//

#include "AI.hpp"

#include <generate/ItemChoice_gen.hpp>
#include <generate/EquipmentChoice_gen.hpp>
#include <generate/GameOperation_gen.hpp>
#include <utility>
#include "util/Logging.hpp"
#include "AICallback.hpp"

AI::AI(std::string address, uint16_t port, const std::string &name, unsigned int verbosity,
       unsigned int difficulty,
       std::map<std::string, std::string> additionalOptions) : address(std::move(address)), port(port), name(name),
                                                               difficulty(difficulty) {

    maxReconnect = additionalOptions.find("maxReconnect") != additionalOptions.end() ? std::stoi(
            additionalOptions.at("maxReconnect")) : 5;

    libClientHandler.emplace(std::dynamic_pointer_cast<libclient::Callback>(std::make_shared<AICallback>(*this)));

    // set up logging
    Logging::initLogging(verbosity);

    connect();
}

void AI::connect() {
    // connect to server as AI
    if (!libClientHandler->network.connect(address, port)) {
        spdlog::critical("could not connect to server");
        exit(2);
    }
    spdlog::info("connected to server");
    if (!libClientHandler->network.sendHello(name, spy::network::RoleEnum::AI)) {
        spdlog::critical("could not send Hello message");
        exit(2);
    }
    spdlog::info("send Hello message");
}

void AI::requestConfigs() {
    using namespace spy::network::messages;
    if (!libClientHandler->network.sendRequestMetaInformation(
            {MetaInformationKey::CONFIGURATION_MATCH_CONFIG, MetaInformationKey::CONFIGURATION_SCENARIO,
             MetaInformationKey::CONFIGURATION_CHARACTER_INFORMATION})) {
        spdlog::critical("could not send RequestMetaInformation message for configs");
        exit(2);
    }
    spdlog::info("send RequestMetaInformation message for configs");
}

void AI::welcomed() {
    requestConfigs();
}

void AI::itemChoice() {
    if (!matchConfig.has_value() || !scenarioConfig.has_value() || !characterConfig.has_value()) {
        spdlog::error("configs for item choice are not available");
        configsWereNotAvailable = true;
        requestConfigs();
        return;
    }
    auto choice = ItemChoice_gen::generate(difficulty, libClientHandler->getOfferedCharacters(),
                                           libClientHandler->getOfferedGadgets(), matchConfig.value(),
                                           scenarioConfig.value(),
                                           characterConfig.value());
    if (!libClientHandler->network.sendItemChoice(choice)) {
        spdlog::critical("could not send ItemChoice_gen message");
        exit(2);
    }
    spdlog::info("sent ItemChoice_gen message");
}

void AI::equipmentChoice() {
    auto equipment = EquipmentChoice_gen::generate(difficulty, libClientHandler->getChosenCharacters(),
                                                   libClientHandler->getChosenGadgets(), matchConfig.value(),
                                                   scenarioConfig.value(),
                                                   characterConfig.value());
    if (!libClientHandler->network.sendEquipmentChoice(equipment)) {
        spdlog::critical("could not send EquipmentChoice_gen message");
        exit(2);
    }
    spdlog::info("sent EquipmentChoice_gen message");
}

void AI::gameStatus() {
    // TODO save additional info (can this be done in LibClient to help Client too) ?
}

void AI::gameOperation() {
    auto operation = GameOperation_gen::generate(difficulty, libClientHandler->getActiveCharacter(),
                                                 libClientHandler->getState(), matchConfig.value());
    if (!libClientHandler->network.sendGameOperation(operation, matchConfig.value())) {
        spdlog::critical("could not send GameOperation_gen message");
        exit(2);
    }
    spdlog::info("sent GameOperation_gen message");
}

void AI::statistics() {
    // could be used as additional information for next runs
}

void AI::metaInformation() {
    using namespace spy::network::messages;
    auto infoMap = libClientHandler->getInformation();
    matchConfig = std::get<spy::MatchConfig>(infoMap.at(MetaInformationKey::CONFIGURATION_MATCH_CONFIG));
    scenarioConfig = std::get<spy::scenario::Scenario>(infoMap.at(MetaInformationKey::CONFIGURATION_SCENARIO));
    characterConfig = std::get<std::vector<spy::character::CharacterInformation>>(infoMap.at(MetaInformationKey::CONFIGURATION_MATCH_CONFIG));

    if (configsWereNotAvailable) {
        configsWereNotAvailable = false;
        itemChoice();
    }
}

void AI::strike() {
    spdlog::warn("ki caused a strike");
}

void AI::error() {
    switch (libClientHandler->getErrorReason().value()) {
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

void AI::replay() {
    // could be used as additional information for next runs
}

void AI::connectionLost() {
    for (unsigned int i = 0; i < maxReconnect; i++) {
        if (libClientHandler->network.sendReconnect()) {
            spdlog::info("sent Reconnect message");
            return;
        }
    }
    spdlog::critical("could not reconnect");
    exit(2);
}


