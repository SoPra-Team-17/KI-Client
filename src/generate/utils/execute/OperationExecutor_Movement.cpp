//
// Created by Carolin on 13.06.2020.
//

#include "OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeMovement(const spy::gameplay::State_AI &state, const spy::gameplay::Movement &op) {
    spy::gameplay::State_AI s = state;
    s.operationsLeadingToState.push_back(std::make_shared<spy::gameplay::Movement>(op));

    auto character = s.getCharacters().getByUUID(op.getCharacterId());
    character->subMovePoint();
    if (character->getMovePoints() == 0 && character->getActionPoints() == 0) {
        s.isLeafState = true;
    }

    // search for character at target position
    auto charTarget = std::find_if(s.getCharacters().begin(), s.getCharacters().end(),
                                   [&op](const spy::character::Character &c) {
                                       return c.getCoordinates() == op.getTarget();
                                   });

    if (charTarget != s.getCharacters().end()) {
        // characters need to swap places
        charTarget->setCoordinates(op.getFrom());
    }

    // special treatment for the cat
    if (s.getCatCoordinates().has_value() && s.getCatCoordinates().value() == op.getTarget()) {
        s.setCatCoordinates(op.getFrom());
    }

    // special treatment for the janitor
    if (s.getJanitorCoordinates().has_value() && s.getJanitorCoordinates().value() == op.getTarget()) {
        s.setJanitorCoordinates(op.getFrom());
    }

    character->setCoordinates(op.getTarget());

    auto gadget = s.getMap().getField(op.getTarget()).getGadget();
    if (gadget.has_value()) {
        // pick up gadget
        character->addGadget(gadget.value());
        s.getMap().getField(op.getTarget()).removeGadget();
        s.collectedGadgets.push_back(gadget.value()->getType());
    }

    return {s};
}