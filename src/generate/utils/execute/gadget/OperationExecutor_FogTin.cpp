//
// Created by Carolin on 13.06.2020.
//

#include <util/GameLogicUtils.hpp>
#include "../OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeFogTin(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op) {
    spy::gameplay::State_AI s = state;

    s.getMap().getField(op.getTarget()).setFoggy(true);
    // get neighboring fields
    auto points = spy::util::GameLogicUtils::getNearFieldsInDist(s, op.getTarget(), 1, [](const spy::util::Point &/*p*/) {
        return true;
    });

    bool characterInFog = spy::util::GameLogicUtils::isPersonOnField(s, op.getTarget());
    if (points.second) {
        for (const auto &p : points.first) {
            characterInFog = characterInFog || spy::util::GameLogicUtils::isPersonOnField(s, p);
            s.getMap().getField(p).setFoggy(true);
            s.foggyFields.push_back(p);
        }
    }
    if (!characterInFog) {
        return {}; // no obvious reason to put fog there
    }

    // remove fog tin from inventory
    auto sourceChar = s.getCharacters().getByUUID(op.getCharacterId());
    sourceChar->removeGadget(spy::gadget::GadgetEnum::FOG_TIN);

    return {s};
}