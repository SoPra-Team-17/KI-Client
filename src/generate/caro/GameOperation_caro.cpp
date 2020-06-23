//
// Created by Carolin on 10.06.2020.
//

#include <generate/utils/State_AI.hpp>
#include "../GameOperation_gen.hpp"
#include <datatypes/gameplay/RetireAction.hpp>
#include <spdlog/spdlog.h>

std::shared_ptr<spy::gameplay::BaseOperation>
GameOperation_gen::caro(const spy::util::UUID &characterId, const spy::gameplay::State &s,
                        const spy::MatchConfig &config,
                        const std::vector<spy::character::CharacterInformation> &characterConfig,
                        const libclient::LibClient &libClient, ApproachHelpers &approachHelper) {
    spy::gameplay::State_AI state{s};

    // time left
    constexpr std::chrono::milliseconds minEvaluationTime = std::chrono::milliseconds(2000);
    std::optional<std::chrono::milliseconds> maxDuration = std::nullopt;
    auto limit = config.getTurnPhaseLimit();
    if (limit.has_value()) {
        maxDuration = std::chrono::milliseconds(limit.value() * 1000);
    }
    if (maxDuration.has_value()) {
        if (maxDuration.value() < minEvaluationTime) {
            spdlog::info("fallback to random as turn phase time is too short");
            return random(characterId, s, config);
        }
        maxDuration = maxDuration.value() - minEvaluationTime;
    }

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
