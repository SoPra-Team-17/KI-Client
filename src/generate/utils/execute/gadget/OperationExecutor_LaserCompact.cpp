//
// Created by Carolin on 13.06.2020.
//

#include <util/GameLogicUtils.hpp>
#include <datatypes/gadgets/Cocktail.hpp>
#include "../OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeLaserCompact(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                                       const spy::MatchConfig &config, const libclient::LibClient &libClient) {
    if (config.getLaserCompactHitChance() == 0) {
        return {};
    }

    std::vector<spy::gameplay::State_AI> honeyStates;
    spy::gameplay::State_AI myState = state;

    auto character = myState.getCharacters().getByUUID(op.getCharacterId());

    myState.modStateChance(*character, config.getBowlerBladeHitChance());

    // honey trap
    auto honeyTrapResult = myState.handleHoneyTrap(op, config, libClient);
    honeyStates = honeyTrapResult.first;
    if (honeyTrapResult.second) {
        return honeyStates;
    }

    // successful usage of laser compact
    bool personOnField = spy::util::GameLogicUtils::isPersonOnField(myState, op.getTarget());
    if (personOnField) {
        auto targetPerson = spy::util::GameLogicUtils::getInCharacterSetByCoordinates(myState.getCharacters(), op.getTarget());
        bool poisoned = std::dynamic_pointer_cast<spy::gadget::Cocktail>(targetPerson->getGadget(spy::gadget::GadgetEnum::COCKTAIL).value())->isPoisoned();
        myState.removedCocktails.push_back({targetPerson->getCharacterId(), poisoned});
        targetPerson->removeGadget(op.getGadget());
    } else {
        // remove cocktail from field
        auto &field = myState.getMap().getField(op.getTarget());
        bool poisoned = std::dynamic_pointer_cast<spy::gadget::Cocktail>(field.getGadget().value())->isPoisoned();
        myState.removedCocktails.push_back({op.getTarget(), poisoned});
        field.removeGadget();
    }

    honeyStates.push_back(myState);
    return honeyStates;
}