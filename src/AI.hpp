//
// Created by Carolin on 16.05.2020.
//

#ifndef KICLIENT_AI_HPP
#define KICLIENT_AI_HPP

#include <spdlog/spdlog.h>
#include <LibClient.hpp>


class AI : public libclient::Callback {
    public:
        AI(std::string address, uint16_t port, std::string name, unsigned int verbosity,
           unsigned int difficulty, std::map<std::string, std::string> additionalOptions);

        void onHelloReply() override;

        void onGameStarted() override;

        void onRequestItemChoice() override;

        void onRequestEquipmentChoice() override;

        void onGameStatus() override;

        void onRequestGameOperation() override;

        [[noreturn]] void onStatistics() override;

        void onGameLeft() override;

        void onGamePause() override;

        void onMetaInformation() override;

        void onStrike() override;

        void onError() override;

        void onReplay() override;

        void connectionLost() override;

        void wrongDestination() override;

    private:
        std::string address;
        uint16_t port;
        std::string name;
        unsigned int difficulty;
        unsigned int maxReconnect;
        bool delay;

        libclient::LibClient libClientHandler;
        std::optional<spy::MatchConfig> matchConfig;
        std::optional<spy::scenario::Scenario> scenarioConfig;
        std::optional<std::vector<spy::character::CharacterInformation>> characterConfig;
        bool itemChoiceRequested = false;
        bool configsInProgress = false;

        void connect();
        void requestConfigs();
};


#endif //KICLIENT_AI_HPP
