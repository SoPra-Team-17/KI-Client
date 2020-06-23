//
// Created by Carolin on 13.06.2020.
//

#include "OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeProperty(const spy::gameplay::State_AI &state, const spy::gameplay::PropertyAction &op,
                                   const spy::MatchConfig &config, const libclient::LibClient &libClient) {
    spy::gameplay::State_AI s = state;
    s.operationsLeadingToState.push_back(std::make_shared<spy::gameplay::PropertyAction>(op));

    auto character = s.getCharacters().getByUUID(op.getCharacterId());
    character->subActionPoint();
    if (character->getMovePoints() == 0 && character->getActionPoints() == 0) {
        s.isLeafState = true;
    }

    switch (op.getUsedProperty()) {
        case spy::character::PropertyEnum::BANG_AND_BURN:
            return OperationExecutor::executeBangAndBurn(s, op);
        case spy::character::PropertyEnum::OBSERVATION:
            return OperationExecutor::executeObservation(s, op, config, libClient);
        default:
            return {};
    }
}
