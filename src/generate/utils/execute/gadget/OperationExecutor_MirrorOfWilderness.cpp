//
// Created by Carolin on 13.06.2020.
//

#include <util/GameLogicUtils.hpp>
#include "../OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI> OperationExecutor::executeMirrorOfWilderness(const spy::gameplay::State_AI &state,
                                                                                  const spy::gameplay::GadgetAction &op,
                                                                                  const spy::MatchConfig &config) {
    spy::gameplay::State_AI sSuccess = state;

    // check if target person is of own faction
    auto sourceChar = sSuccess.getCharacters().getByUUID(op.getCharacterId());
    auto targetChar = spy::util::GameLogicUtils::getInCharacterSetByCoordinates(sSuccess.getCharacters(), op.getTarget());

    bool sameFaction = (sourceChar->getFaction() == targetChar->getFaction());
    if (!sameFaction) {
        if (config.getMirrorSwapChance() == 0) {
            return {};
        }
        sSuccess.modStateChance(*sourceChar, config.getMirrorSwapChance());
    }

    // TODO (optional): only working correct for sameFacion as ip are currently not tracked by libClient object
    auto sourceCharIP = sourceChar->getIntelligencePoints();
    auto targetCharIP = targetChar->getIntelligencePoints();
    sourceChar->setIntelligencePoints(targetCharIP);
    targetChar->setIntelligencePoints(sourceCharIP);

    int ipDiff = static_cast<int>(sourceChar->getIntelligencePoints() - targetChar->getIntelligencePoints());
    sSuccess.mowResult.first = ipDiff;

    // check and apply wiretap with earplugs gadget
    if (ipDiff > 0) {
        // source has more ip than target after change -> due to change source received ip
        auto character = spy::util::GameLogicUtils::getWiredCharacter(sSuccess, *sourceChar);
        if (character.has_value()) {
            character.value()->addIntelligencePoints(ipDiff);
            sSuccess.mowResult.second = character.value()->getCharacterId();
        }
    } else if (ipDiff < 0) {
        // target has more ip than source after change -> due to change target received ip
        auto character = spy::util::GameLogicUtils::getWiredCharacter(sSuccess, *targetChar);
        if (character.has_value()) {
            character.value()->addIntelligencePoints(std::abs(ipDiff));
            sSuccess.mowResult.second = character.value()->getCharacterId();
        }
    }


    if (!sameFaction) {
        sourceChar->removeGadget(spy::gadget::GadgetEnum::MIRROR_OF_WILDERNESS);
    }

    return {sSuccess};
}