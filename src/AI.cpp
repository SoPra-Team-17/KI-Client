//
// Created by Carolin on 16.05.2020.
//

#include "AI.hpp"

#include <generate/ItemChoice_gen.hpp>
#include <generate/EquipmentChoice_gen.hpp>
#include <generate/GameOperation_gen.hpp>
#include <utility>
#include <random>
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
        throw std::runtime_error{"could not connect to server"};
    }
    spdlog::info("connected to server");
    if (!libClientHandler.network.sendHello(name, spy::network::RoleEnum::AI)) {
        throw std::runtime_error{"could not send Hello message"};
    }
    spdlog::info("send Hello message");
}

void AI::requestConfigs() {
    if (!configsInProgress) {
        using namespace spy::network::messages;
        if (!libClientHandler.network.sendRequestMetaInformation(
                {MetaInformationKey::CONFIGURATION_MATCH_CONFIG, MetaInformationKey::CONFIGURATION_SCENARIO,
                 MetaInformationKey::CONFIGURATION_CHARACTER_INFORMATION})) {
            throw std::runtime_error{"could not send RequestMetaInformation message for configs"};
        }
        configsInProgress = true;
        spdlog::info("send RequestMetaInformation message for configs");
    }
    std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(5));
    if (!configsInProgress) {
        requestConfigs();
    }
}

void AI::onHelloReply() {
    spdlog::info("received HelloReply message");

    requestConfigs();
}

void AI::onGameStarted() {
    spdlog::info("received GameStarted message");
    // do nothing
}

void AI::onRequestItemChoice() {
    spdlog::info("received RequestItemChoice message");

    if (!matchConfig.has_value() || !scenarioConfig.has_value() || !characterConfig.has_value()) {
        spdlog::error("configs for item choice are not available");
        itemChoiceRequested = true;
        requestConfigs();
        return;
    }
    auto choice = ItemChoice_gen::generate(difficulty, libClientHandler.getOfferedCharacters(),
                                           libClientHandler.getOfferedGadgets(), matchConfig.value(),
                                           scenarioConfig.value(),
                                           characterConfig.value());
    if (!libClientHandler.network.sendItemChoice(choice)) {
        throw std::runtime_error{"could not send ItemChoice_gen message"};
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
        throw std::runtime_error{"could not send EquipmentChoice_gen message"};
    }
    spdlog::info("sent EquipmentChoice message");
}

void AI::onGameStatus() {
    spdlog::info("received GameStatus message");

    // additional info could be saved here (can this be done in LibClient to help Client too) ?
}

void AI::onRequestGameOperation() {
    spdlog::info("received RequestGameOperation message");

    auto waitTime = std::chrono::milliseconds(1000);
    auto limit = matchConfig.value().getTurnPhaseLimit();
    if (limit.has_value() && limit.value() > 1) {
        waitTime = std::chrono::milliseconds((limit.value() - 1) * 1000);
    }

    auto start = std::chrono::high_resolution_clock::now();
    if (nextOperations.empty()) {
        nextOperations = GameOperation_gen::generate(difficulty, libClientHandler.getActiveCharacter(),
                                                     libClientHandler.getState(), matchConfig.value());
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto usedTime = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    if (waitTime - usedTime > std::chrono::milliseconds(1000)) {
        auto diff = waitTime - usedTime;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<unsigned int> randTime(0, diff.count());
        std::this_thread::sleep_for(std::chrono::milliseconds(randTime(gen)));
    } else {
        spdlog::warn("took too long to calculate");
    }

    if (!libClientHandler.network.sendGameOperation(nextOperations[0], matchConfig.value())) {
        throw std::runtime_error{"could not send GameOperation_gen message"};
    }
    nextOperations.erase(nextOperations.begin());
    spdlog::info("sent GameOperation message");
}

void AI::onStatistics() {
    spdlog::info("received Statistics message");

    // additional info (for next runs) could be saved here
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
    try {
        matchConfig = std::get<spy::MatchConfig>(infoMap.at(MetaInformationKey::CONFIGURATION_MATCH_CONFIG));
        scenarioConfig = std::get<spy::scenario::Scenario>(infoMap.at(MetaInformationKey::CONFIGURATION_SCENARIO));
        characterConfig = std::get<std::vector<spy::character::CharacterInformation>>(
                infoMap.at(MetaInformationKey::CONFIGURATION_CHARACTER_INFORMATION));

        configsInProgress = false;
        if (itemChoiceRequested) {
            itemChoiceRequested = false;
            onRequestItemChoice();
        }
    } catch (std::out_of_range &e) {
        requestConfigs();
    }
}

void AI::onStrike() {
    spdlog::info("received Strike message");

    configsInProgress = false;
    spdlog::warn("ki caused a strike");
}

void AI::onError() {
    spdlog::info("received Error message");

    configsInProgress = false;
    switch (libClientHandler.getErrorReason().value()) {
        case spy::network::ErrorTypeEnum::NAME_NOT_AVAILABLE:
            spdlog::error("error type is name not available");
            if (name == "ki017") {
                throw std::runtime_error{"default name is not available"};
            }
            name = "ki017";
            connect();
            break;
        case spy::network::ErrorTypeEnum::ALREADY_SERVING:
            throw std::runtime_error{"error type is already serving"};
        case spy::network::ErrorTypeEnum::SESSION_DOES_NOT_EXIST:
            throw std::runtime_error{"error type is session does not exist"};
        case spy::network::ErrorTypeEnum::ILLEGAL_MESSAGE:
            throw std::runtime_error{"error type is illegal message"};
        case spy::network::ErrorTypeEnum::TOO_MANY_STRIKES:
            throw std::runtime_error{"error type is too many strikes"};
        case spy::network::ErrorTypeEnum::GENERAL:
            [[fallthrough]];
        default:
            throw std::runtime_error{"error type is general or unknown"};
    }
}

void AI::onReplay() {
    spdlog::debug("received Replay message");
    // do nothing
}

void AI::connectionLost() {
    spdlog::info("received connection lost callback");

    unsigned int waitTime = 1000;
    auto limit = matchConfig.value().getReconnectLimit();
    if (limit.has_value() && limit.value() > 1) {
        waitTime = (limit.value() - 1) * 1000 / maxReconnect;
    }

    configsInProgress = false;
    for (unsigned int i = 0; i < maxReconnect; i++) {
        if (libClientHandler.network.sendReconnect()) {
            spdlog::info("sent Reconnect message");
            return;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
    }
    throw std::runtime_error{"could not reconnect"};
}

void AI::wrongDestination() {
    spdlog::debug("received message that was not meant for me");
    // do nothing
}


