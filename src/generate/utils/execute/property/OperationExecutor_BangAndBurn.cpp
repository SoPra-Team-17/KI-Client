//
// Created by Carolin on 13.06.2020.
//

#include "../OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeBangAndBurn(const spy::gameplay::State_AI &state, const spy::gameplay::PropertyAction &op) {
    spy::gameplay::State_AI s = state;

    s.getMap().getField(op.getTarget()).setDestroyed(true);
    s.destroyedRoulettes.push_back(op.getTarget());

    return {s};
}