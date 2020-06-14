//
// Created by Carolin on 13.06.2020.
//

#include <util/GameLogicUtils.hpp>
#include "../OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeChickenFeed(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                                      const libclient::LibClient &libClient) {
    spy::gameplay::State_AI s = state;

    auto sourceChar = s.getCharacters().getByUUID(op.getCharacterId());
    auto targetChar = spy::util::GameLogicUtils::getInCharacterSetByCoordinates(s.getCharacters(), op.getTarget());
    // remove gadget
    sourceChar->removeGadget(op.getGadget());

    auto isTargetEnemy = libClient.hasCharacterFaction(targetChar->getCharacterId(),
                                                       spy::character::FactionEnum::PLAYER2);

    if (isTargetEnemy.has_value()) {
        if (isTargetEnemy.value() == 0) {
            return {};
        } else {
            s.chickenfeedResult.first = isTargetEnemy.value();
        }
    } else {
        s.chickenfeedResult.first = 0.5;
    }
    // TODO (optional): ipDiff but ip are currently not tracked by libClient object

    // check wiretap with earplugs gadget
    auto character = spy::util::GameLogicUtils::getWiredCharacter(s, *sourceChar);
    if (character.has_value()) {
        s.chickenfeedResult.second = character.value()->getCharacterId();
    } else {
        s.chickenfeedResult.second = std::nullopt;
    }

    return {s};
}