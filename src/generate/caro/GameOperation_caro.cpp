//
// Created by Carolin on 10.06.2020.
//

#include <generate/utils/State_AI.hpp>
#include "../GameOperation_gen.hpp"
#include "evalFunctions_caro.hpp"

std::shared_ptr<spy::gameplay::BaseOperation>
GameOperation_gen::caro(const spy::util::UUID &characterId, const spy::gameplay::State &s,
                        const spy::MatchConfig &config,
                        const std::vector<spy::character::CharacterInformation> &characterConfig,
                        const libclient::LibClient &libClient, ApproachHelpers &approachHelper) {
    spy::gameplay::State_AI state{s};
    auto endStates = state.getLeafSuccessorStates(characterId, config, libClient);

    std::shared_ptr<spy::gameplay::BaseOperation> operationToExecute;
    double endVal = -std::numeric_limits<double>::infinity();
    for (auto &endS: endStates) {
        double val = approachHelper.evalFun_caro.value().gameOperation(state, endS, characterId, config,
                                                       characterConfig,
                                                       libClient);
        if (val > endVal) {
            endVal = val;
            operationToExecute = endS.operationsLeadingToState.at(0);
        }
    }

    return operationToExecute;
}
