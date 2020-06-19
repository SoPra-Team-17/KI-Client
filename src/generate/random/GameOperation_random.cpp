//
// Created by Carolin on 17.05.2020.
//

#include "../GameOperation_gen.hpp"
#include <util/GameLogicUtils.hpp>
#include <generate/utils/generate/OperationGenerator.hpp>

std::shared_ptr<spy::gameplay::BaseOperation>
GameOperation_gen::random(const spy::util::UUID &characterId, const spy::gameplay::State &s,
                          const spy::MatchConfig &config) {
    spy::gameplay::State_AI state {s};
    return *spy::util::GameLogicUtils::getRandomItemFromContainer(OperationGenerator::generate(state, characterId, config));
}