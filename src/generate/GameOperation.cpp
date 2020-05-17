//
// Created by Carolin on 17.05.2020.
//

#include "GameOperation.hpp"

std::shared_ptr<spy::gameplay::BaseOperation> GameOperation::generate(unsigned int difficulty, const spy::util::UUID &characterId, const spy::gameplay::State &s,
                             const spy::MatchConfig &config) {
    switch (difficulty) {
        case 1:
            return random(characterId, s, config);
        default:
            throw std::domain_error("undefined difficulty");
    }
}
