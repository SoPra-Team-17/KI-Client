//
// Created by Carolin on 13.06.2020.
//

#include "../OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeJetpack(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op) {
    spy::gameplay::State_AI s = state;

    auto character = s.getCharacters().getByUUID(op.getCharacterId());
    if (spy::gameplay::Movement::getMoveDistance(character->getCoordinates().value(), op.getTarget()) <=
        character->getMovePoints()) {
        return {}; // you can move there
    }
    character->setCoordinates(op.getTarget());
    character->removeGadget(op.getGadget());

    return {s};
}