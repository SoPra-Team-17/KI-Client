//
// Created by Carolin on 16.05.2020.
//

#ifndef KICLIENT_AI_HPP
#define KICLIENT_AI_HPP

#include <spdlog/spdlog.h>
#include <LibClient.hpp>

class AI {
    public:
        AI(std::string address, uint16_t port, const std::string &name, unsigned int verbosity,
           unsigned int difficulty, std::map<std::string, std::string> additionalOptions);

        void welcomed();

        void itemChoice();

        void equipmentChoice();

        void gameStatus();

        void gameOperation();

        void statistics();

        void metaInformation();

        void strike();

        void error();

        void replay();

        void connectionLost();

    private:
        std::string address;
        uint16_t port;
        std::string name;
        unsigned int difficulty;
        unsigned int maxReconnect;

        std::optional<libclient::LibClient> libClientHandler;
        std::optional<spy::MatchConfig> matchConfig;
        std::optional<spy::scenario::Scenario> scenarioConfig;
        std::optional<std::vector<spy::character::CharacterInformation>> characterConfig;
        bool configsWereNotAvailable = false;

        void connect();
        void requestConfigs();
};


#endif //KICLIENT_AI_HPP
