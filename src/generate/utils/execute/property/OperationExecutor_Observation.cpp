//
// Created by Carolin on 13.06.2020.
//

#include <util/GameLogicUtils.hpp>
#include "../OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeObservation(const spy::gameplay::State_AI &state, const spy::gameplay::PropertyAction &op,
                                      const spy::MatchConfig &config, const libclient::LibClient &libClient) {
    spy::gameplay::State_AI sSuccess = state;

    auto sourceChar = state.getCharacters().findByUUID(op.getCharacterId());
    auto targetChar = spy::util::GameLogicUtils::findInCharacterSetByCoordinates(state.getCharacters(), op.getTarget());

    if (sourceChar->getFaction() == targetChar->getFaction() ||
        libClient.getUnknownFactionList().find(targetChar->getCharacterId()) ==
        libClient.getUnknownFactionList().end() || config.getObservationSuccessChance() == 0) {
        return {}; // action is not useful (no new information will be gained) -> do not continue branch
    } else {
        sSuccess.modStateChance(*sourceChar, config.getObservationSuccessChance());
    }

    auto targetHasPocketLitter = libClient.hasCharacterGadget(targetChar->getCharacterId(),
                                                              spy::gadget::GadgetEnum::POCKET_LITTER);
    if (targetHasPocketLitter.has_value()) {
        if (targetHasPocketLitter.value() == 1) {
            return {}; // action is not useful (no new information will be gained) -> do not continue branch
        }
        sSuccess.observationResult.emplace_back(1 - targetHasPocketLitter.value());
    } else {
        sSuccess.observationResult.emplace_back(std::nullopt);
    }

    return {sSuccess};
}