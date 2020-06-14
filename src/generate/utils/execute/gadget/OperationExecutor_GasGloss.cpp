//
// Created by Carolin on 13.06.2020.
//

#include <util/GameLogicUtils.hpp>
#include "../OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeGasGloss(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                                   const spy::MatchConfig &config, const libclient::LibClient &libClient) {
    std::vector<spy::gameplay::State_AI> honeyStates;
    spy::gameplay::State_AI myState = state;

    auto character = myState.getCharacters().getByUUID(op.getCharacterId());

    // honey trap and babysitter
    auto honeyTrapResult = myState.handleHoneyTrap(op, config, libClient);
    honeyStates = honeyTrapResult.first;
    if (honeyTrapResult.second) {
        return honeyStates;
    }
    if (myState.handleBabysitter(op, config)) {
        return {};
    }

    // successful usage of gas gloss
    auto targetPerson = spy::util::GameLogicUtils::getInCharacterSetByCoordinates(myState.getCharacters(), op.getTarget());
    auto damage = config.getGasGlossDamage();
    spy::util::GameLogicUtils::applyDamageToCharacter(myState, *targetPerson, damage);
    myState.addDamage(*targetPerson, damage);

    // remove gas gloss from inventory
    character->removeGadget(op.getGadget());

    honeyStates.push_back(myState);
    return honeyStates;
}