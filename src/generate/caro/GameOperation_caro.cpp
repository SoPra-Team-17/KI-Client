//
// Created by Carolin on 10.06.2020.
//

#include <generate/utils/State_AI.hpp>
#include "../GameOperation_gen.hpp"
#include "evalFunctions_caro.hpp"

std::shared_ptr<spy::gameplay::BaseOperation>
GameOperation_gen::caro(const spy::util::UUID &characterId, const spy::gameplay::State &s,
                        const spy::MatchConfig &config) {
    spy::gameplay::State_AI state {s};
    auto endStates = state.getLeafSuccessorStates(characterId, config);

    std::shared_ptr<spy::gameplay::BaseOperation> operationToExecute;
    double endVal = -std::numeric_limits<double>::infinity();
    for (const auto &endS: endStates) {
        double val = evalFunctions_caro::gameOperation(endS, characterId);
        if (val > endVal) {
            endVal = val;
            operationToExecute = endS.operationsLeadingToState[0];
        }
    }

    return operationToExecute;
}
