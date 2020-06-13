//
// Created by Carolin on 13.06.2020.
//

#include <datatypes/gadgets/WiretapWithEarplugs.hpp>
#include <util/GameLogicUtils.hpp>
#include "../OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI> OperationExecutor::executeWiretapWithEarplugs(const spy::gameplay::State_AI &state,
                                                                                   const spy::gameplay::GadgetAction &op) {
    spy::gameplay::State_AI s = state;

    auto sourceChar = s.getCharacters().getByUUID(op.getCharacterId());
    auto targetChar = spy::util::GameLogicUtils::getInCharacterSetByCoordinates(s.getCharacters(), op.getTarget());
    auto gadget = std::dynamic_pointer_cast<spy::gadget::WiretapWithEarplugs>(sourceChar->getGadget(op.getGadget()).value());
    gadget->setWorking(true);
    gadget->setActiveOn(targetChar->getCharacterId());

    return {s};
}