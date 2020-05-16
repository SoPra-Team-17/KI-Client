//
// Created by Carolin on 16.05.2020.
//

#include "AI.hpp"

#include <utility>
#include "util/Logging.hpp"
#include "AICallback.hpp"

AI::AI(const std::string &address, uint16_t port, const std::string &name, unsigned int verbosity,
       unsigned int difficulty,
       std::map<std::string, std::string> additionalOptions) : address(address), port(port), name(name),
                                                                      verbosity(verbosity), difficulty(difficulty),
                                                                      additionalOptions(std::move(additionalOptions)) {

    libClientHandler.emplace(std::dynamic_pointer_cast<libclient::Callback>(std::make_shared<AICallback>(*this)));

    // set up logging
    Logging::initLogging(verbosity);

    // connect to server as AI
    if (libClientHandler->network.connect(address, port)) {
        bool worked = libClientHandler->network.sendHello(name, spy::network::RoleEnum::AI);
        if (!worked) {
            throw std::domain_error("could not send HELLO message");
        }
    }
    throw std::domain_error("could not connect to server");
}

void AI::itemChoice() {
    // TODO generate and send ItemChoice (with difficulty)
}

void AI::equipmentChoice() {
    // TODO generate and send EquipmentChoice (with difficulty)
}

void AI::gameStatus() {
    // TODO save additional info or can this be done in LibClient to help Client too ?
}

void AI::gameOperation() {
    // TODO generate and  send GameOperation (with difficulty)
}

void AI::statistics() {
    // TODO use additional info ?
}

void AI::metaInformation() {
    // TODO what to do with received information ?
    // TODO MatchConfig is needed !!!
}

void AI::strike() {
    // TODO how to handle strike ?
}

void AI::error() {
    // TODO how to handle different errors ?
}

void AI::replay() {
    // TODO use additional info ?
}

void AI::connectionLost() {
    // TODO try to reconnect
}


