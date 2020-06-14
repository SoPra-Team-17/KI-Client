//
// Created by Carolin on 13.06.2020.
//

#include "../OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI> OperationExecutor::executeTechnicolorPrism(const spy::gameplay::State_AI &state,
                                                                                const spy::gameplay::GadgetAction &op) {
    spy::gameplay::State_AI s = state;

    // invert roulette table
    s.getMap().getField(op.getTarget()).setInverted(true);
    s.invertedRoulette = op.getTarget();

    // remove technicolor prism from inventory
    auto character = s.getCharacters().getByUUID(op.getCharacterId());
    character->removeGadget(spy::gadget::GadgetEnum::TECHNICOLOUR_PRISM);

    return {s};
}