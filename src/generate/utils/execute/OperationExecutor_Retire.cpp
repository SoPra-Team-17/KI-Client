//
// Created by Carolin on 13.06.2020.
//

#include "OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeRetire(const spy::gameplay::State_AI &state, const spy::gameplay::RetireAction &op) {
    spy::gameplay::State_AI s = state;
    s.operationsLeadingToState.push_back(std::make_shared<spy::gameplay::RetireAction>(op));

    auto character = s.getCharacters().getByUUID(op.getCharacterId());
    character->setActionPoints(0);
    character->setMovePoints(0);
    s.isLeafState = true;

    return {s};
}
