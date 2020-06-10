//
// Created by Carolin on 10.06.2020.
//

#include <generate/utils/State_AI.hpp>
#include "../GameOperation_gen.hpp"
#include "evalFunctions_caro.hpp"

std::vector<std::shared_ptr<spy::gameplay::BaseOperation>>
GameOperation_gen::caro(const spy::util::UUID &characterId, const spy::gameplay::State &s,
                        const spy::MatchConfig &config) {
    spy::gameplay::State_AI state {s};
    auto endStates = state.getLeafSuccessorStates(characterId, config);

    std::vector<std::shared_ptr<spy::gameplay::BaseOperation>> operationsToExecute;
    double endVal = -1;
    for (const auto &endS: endStates) {
        double val = evalFunctions_caro::gameOperation(endS, characterId);
        if (val > endVal) {
            operationsToExecute = endS.operationsLeadingToState;
        }
    }

    return operationsToExecute;
}
