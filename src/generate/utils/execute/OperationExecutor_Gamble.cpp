//
// Created by Carolin on 13.06.2020.
//

#include <util/GameLogicUtils.hpp>
#include "OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeGamble(const spy::gameplay::State_AI &state, const spy::gameplay::GambleAction &op) {
    spy::gameplay::State_AI sWon = state;

    sWon.operationsLeadingToState.push_back(std::make_shared<spy::gameplay::GambleAction>(op));

    auto character = sWon.getCharacters().getByUUID(op.getCharacterId());
    character->subActionPoint();
    if (character->getMovePoints() == 0 && character->getActionPoints() == 0) {
        sWon.isLeafState = true;
    }

    // get winning chance
    auto targetField = sWon.getMap().getField(op.getTarget());
    double winningChance = 18.0/37.0;
    if (character->hasProperty(spy::character::PropertyEnum::LUCKY_DEVIL)) {
        winningChance = 32.0/37.0;
    } else if (character->hasProperty(spy::character::PropertyEnum::JINX)) {
        winningChance = 13.0/37.0;
    }
    winningChance = targetField.isInverted() ? (1-winningChance) : winningChance;
    sWon.modStateChance(*character, winningChance);

    // won in roulette
    character->setChips(character->getChips() + op.getStake());
    sWon.chipDiff += static_cast<int>(op.getStake());
    targetField.setChipAmount(targetField.getChipAmount().value() - op.getStake());


    return {sWon};
}
