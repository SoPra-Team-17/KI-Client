//
// Created by Carolin on 10.06.2020.
//

#include <generate/utils/State_AI.hpp>
#include "../GameOperation_gen.hpp"
#include <datatypes/gameplay/RetireAction.hpp>

std::shared_ptr<spy::gameplay::BaseOperation>
GameOperation_gen::caro(const spy::util::UUID &characterId, const spy::gameplay::State &s,
                        const spy::MatchConfig &config,
                        const std::vector<spy::character::CharacterInformation> &characterConfig,
                        const libclient::LibClient &libClient, ApproachHelpers &approachHelper) {
    spy::gameplay::State_AI state{s};

    // time left
    auto dur = config.getTurnPhaseLimit().value();
    if (dur < 2) {
        return random(characterId, s, config);
    }
    unsigned int maxDuration = (dur - 2) * 1000;

    auto endStates = state.getLeafSuccessorStates(characterId, config, libClient, maxDuration);

    std::shared_ptr<spy::gameplay::BaseOperation> operationToExecute = std::make_shared<spy::gameplay::RetireAction>(
            characterId);
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
