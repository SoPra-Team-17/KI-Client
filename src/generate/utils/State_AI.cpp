//
// Created by Carolin on 10.06.2020.
//

#include "State_AI.hpp"

namespace spy::gameplay {

    std::vector<State_AI> State_AI::getSuccessorStates(const util::UUID &/*characterId*/,
                                                       const spy::MatchConfig &/*config*/) {
        // TODO implement getSuccessorStates method
        return {};
    }

    std::vector<State_AI>
    State_AI::getLeafSuccessorStates(const util::UUID &/*characterId*/,
                                     const spy::MatchConfig &/*config*/) {
        // TODO implement getLeafSuccessorStates method
        return {};
    }
}