//
// Created by Carolin on 17.05.2020.
//

#include "../GameOperation_gen.hpp"

std::shared_ptr<spy::gameplay::BaseOperation>
GameOperation_gen::random(const spy::util::UUID &characterId, const spy::gameplay::State &s,
                          const spy::MatchConfig &config) {
    return {spy::gameplay::ActionGenerator::generateRandomAction(s, characterId, config)};
}