//
// Created by Carolin on 13.06.2020.
//

#include <util/GameLogicUtils.hpp>
#include <datatypes/gadgets/Cocktail.hpp>
#include "../OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executePoisonPills(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                                      const spy::MatchConfig &config, const libclient::LibClient &libClient) {
    std::vector<spy::gameplay::State_AI> honeyStates;
    spy::gameplay::State_AI myState = state;

    // honey trap
    auto honeyTrapResult = myState.handleHoneyTrap(op, config, libClient);
    honeyStates = honeyTrapResult.first;
    if (honeyTrapResult.second) {
        return honeyStates;
    }

    // check if person on field
    bool personOnField = spy::util::GameLogicUtils::isPersonOnField(myState, op.getTarget());
    if (personOnField) {
        // person has cocktail (validate)
        auto targetPerson = spy::util::GameLogicUtils::getInCharacterSetByCoordinates(myState.getCharacters(), op.getTarget());
        auto cocktail = std::dynamic_pointer_cast<spy::gadget::Cocktail>(
                targetPerson->getGadget(spy::gadget::GadgetEnum::COCKTAIL).value());
        cocktail->setIsPoisoned(true);
        myState.poisonedCocktails.push_back(targetPerson->getCharacterId());
    } else {
        // field has cocktail (validate)
        auto cocktail = std::dynamic_pointer_cast<spy::gadget::Cocktail>(
                myState.getMap().getField(op.getTarget()).getGadget().value());
        cocktail->setIsPoisoned(true);
        myState.poisonedCocktails.push_back(op.getTarget());
    }

    // reduce usages of gadget
    auto character = myState.getCharacters().getByUUID(op.getCharacterId());
    auto poisonPills = character->getGadget(spy::gadget::GadgetEnum::POISON_PILLS);
    poisonPills.value()->setUsagesLeft(poisonPills.value()->getUsagesLeft().value() - 1);

    if (poisonPills.value()->getUsagesLeft().value() == 0) {
        character->removeGadget(spy::gadget::GadgetEnum::POISON_PILLS);
    }

    honeyStates.push_back(myState);
    return honeyStates;
}