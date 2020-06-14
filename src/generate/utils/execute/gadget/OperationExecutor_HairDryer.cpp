//
// Created by Carolin on 13.06.2020.
//

#include <util/GameLogicUtils.hpp>
#include "../OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeHairDryer(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op) {
    spy::gameplay::State_AI s = state;

    // Search character at target location
    auto charTarget = spy::util::GameLogicUtils::getInCharacterSetByCoordinates(s.getCharacters(), op.getTarget());

    // Remove CLAMMY_CLOTHES property
    if (charTarget->getProperties().erase(spy::character::PropertyEnum::CLAMMY_CLOTHES) > 0) {
        s.removedClammyClothes.insert(charTarget->getCharacterId());
    } else {
        return {}; // action is not useful (nothing will happen) -> do not continue branch
    }

    return {s};
}