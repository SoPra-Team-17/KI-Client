//
// Created by Carolin on 13.06.2020.
//

#include "OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeSpy(const spy::gameplay::State_AI &state, const spy::gameplay::SpyAction &op,
                              const spy::MatchConfig &config, const libclient::LibClient &libClient) {
    spy::gameplay::State_AI s = state;
    s.operationsLeadingToState.push_back(std::make_shared<spy::gameplay::SpyAction>(op));

    auto character = s.getCharacters().getByUUID(op.getCharacterId());
    character->subActionPoint();
    if (character->getMovePoints() == 0 && character->getActionPoints() == 0) {
        s.isLeafState = true;
    }

    // TODO execute

    return {s};
}
