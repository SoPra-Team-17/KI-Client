//
// Created by Carolin on 13.06.2020.
//

#include <util/GameLogicUtils.hpp>
#include "../OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeGrapple(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                                  const spy::MatchConfig &config) {
    spy::gameplay::State_AI sSuccess = state;
    spy::gameplay::State_AI sFailure = state;

    auto character = sSuccess.getCharacters().getByUUID(op.getCharacterId());

    // successful hit
    auto targetField = sSuccess.getMap().getField(op.getTarget());
    character->addGadget(targetField.getGadget().value());
    sSuccess.grappledGadgets.insert(targetField.getGadget().value()->getType());
    targetField.removeGadget();

    sSuccess.stateChance *= config.getGrappleHitChance();
    sFailure.stateChance *= (1 - config.getGrappleHitChance());

    return {sSuccess, sFailure};
}