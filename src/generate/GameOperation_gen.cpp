//
// Created by Carolin on 17.05.2020.
//

#include "GameOperation_gen.hpp"

std::shared_ptr<spy::gameplay::BaseOperation>
GameOperation_gen::generate(unsigned int difficulty, const spy::util::UUID &characterId, const spy::gameplay::State &s,
                            const spy::MatchConfig &config,
                            const spy::scenario::Scenario &scenarioConfig,
                            const std::vector<spy::character::CharacterInformation> &characterConfig,
                            const libclient::LibClient &libClient) {
    switch (difficulty) {
        case 1:
            return random(characterId, s, config);
        case 2:
            return caro(characterId, s, config, scenarioConfig, characterConfig, libClient);
        default:
            throw std::domain_error("undefined difficulty");
    }
}
