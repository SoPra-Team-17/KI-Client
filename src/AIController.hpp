//
// Created by Carolin on 16.05.2020.
//

#ifndef KICLIENT_AICONTROLLER_HPP
#define KICLIENT_AICONTROLLER_HPP

#include <spdlog/spdlog.h>
#include <LibClient.hpp>
#include "AI.hpp"

class AIController : public libclient::Callback {
    public:
        AIController(const std::string &address, uint16_t port, const std::string &name, unsigned int verbosity,
                     unsigned int difficulty, std::map<std::string, std::string> additionalOptions);

        void onHelloReply() override;

        void onGameStarted() override;

        void onRequestItemChoice() override;

        void onRequestEquipmentChoice() override;

        void onGameStatus() override;

        void onRequestGameOperation() override;

        void onStatistics() override;

        void onGameLeft() override;

        void onGamePause() override;

        void onMetaInformation() override;

        void onStrike() override;

        void onError() override;

        void onReplay() override;

        void connectionLost() override;

        void wrongDestination() override;

    private:
        libclient::LibClient libClientHandler;
        AI ai;
};


#endif //KICLIENT_AICONTROLLER_HPP
