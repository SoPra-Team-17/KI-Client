//
// Created by Carolin on 13.06.2020.
//

#include <util/GameLogicUtils.hpp>
#include "OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeGamble(const spy::gameplay::State_AI &state, const spy::gameplay::GambleAction &op) {
    spy::gameplay::State_AI s = state;
    s.operationsLeadingToState.push_back(std::make_shared<spy::gameplay::GambleAction>(op));

    auto character = s.getCharacters().getByUUID(op.getCharacterId());
    character->subActionPoint();
    if (character->getMovePoints() == 0 && character->getActionPoints() == 0) {
        s.isLeafState = true;
    }

    auto targetField = s.getMap().getField(op.getTarget());

    double winningChance = 18.0/37.0;
    if (character->hasProperty(spy::character::PropertyEnum::LUCKY_DEVIL)) {
        winningChance = 32.0/37.0;
    } else if (character->hasProperty(spy::character::PropertyEnum::JINX)) {
        winningChance = 13.0/37.0;
    }
    winningChance = targetField.isInverted() ? (1-winningChance) : winningChance;

    spy::gameplay::State_AI sWon = s;
    spy::gameplay::State_AI sLost = s;


    // won in roulette
    character->setChips(character->getChips() + op.getStake());
    sWon.chipDiff += static_cast<int>(op.getStake());
    targetField.setChipAmount(targetField.getChipAmount().value() - op.getStake());

    // lost in roulette
    character->setChips(character->getChips() - op.getStake());
    sLost.chipDiff -= static_cast<int>(op.getStake());
    targetField.setChipAmount(targetField.getChipAmount().value() + op.getStake());

    sWon.stateChance *= winningChance;
    sLost.stateChance *= (1 - winningChance);

    return {sWon, sLost};
}
