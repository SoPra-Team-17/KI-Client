//
// Created by Carolin on 13.06.2020.
//

#include "../OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeDiamondCollar(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op) {
    spy::gameplay::State_AI s = state;
    s.isLeafState = true;

    auto character = s.getCharacters().getByUUID(op.getCharacterId());
    s.getFactionStats().collarToCat = character->getFaction();
    s.setHasCatDiamondCollar(true);

    character->removeGadget(spy::gadget::GadgetEnum::DIAMOND_COLLAR);

    return {s};
}