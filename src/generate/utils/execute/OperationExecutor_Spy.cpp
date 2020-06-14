//
// Created by Carolin on 13.06.2020.
//

#include <util/GameLogicUtils.hpp>
#include "OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeSpy(const spy::gameplay::State_AI &state, const spy::gameplay::SpyAction &op,
                              const spy::MatchConfig &config, const libclient::LibClient &libClient) {
    if (config.getSpySuccessChance() == 0) {
        return{};
    }

    spy::gameplay::State_AI s = state;
    s.operationsLeadingToState.push_back(std::make_shared<spy::gameplay::SpyAction>(op));

    auto character = s.getCharacters().getByUUID(op.getCharacterId());
    character->subActionPoint();
    if (character->getMovePoints() == 0 && character->getActionPoints() == 0) {
        s.isLeafState = true;
    }

    auto targetChar = spy::util::GameLogicUtils::findInCharacterSetByCoordinates(s.getCharacters(), op.getTarget());
    if (targetChar != s.getCharacters().end()) {
        // spy on person
        s.modStateChance(*character, config.getSpySuccessChance());

        // is target npc
        auto isTargetNpc = libClient.hasCharacterFaction(targetChar->getCharacterId(),
                                                         spy::character::FactionEnum::NEUTRAL);
        if (isTargetNpc.has_value()) {
            if (isTargetNpc.value() == 0) {
                return {};
            } else {
                s.spyResult.emplace_back(targetChar->getCharacterId(), isTargetNpc.value());
            }
        } else {
            s.spyResult.emplace_back(targetChar->getCharacterId(), std::nullopt);
        }
    } else {
        // spy on safe
        s.spyedSafes.push_back(op.getTarget());
    }

    return {s};
}
