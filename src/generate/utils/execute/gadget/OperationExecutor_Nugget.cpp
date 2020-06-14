//
// Created by Carolin on 13.06.2020.
//

#include <util/GameLogicUtils.hpp>
#include "../OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeNugget(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                                 const libclient::LibClient &libClient) {
    spy::gameplay::State_AI s = state;

    // remove nugget from character
    auto character = s.getCharacters().getByUUID(op.getCharacterId());
    character->removeGadget(spy::gadget::GadgetEnum::NUGGET);

    // get person on target field
    auto person = spy::util::GameLogicUtils::getInCharacterSetByCoordinates(s.getCharacters(), op.getTarget());

    auto personIsNpc = libClient.hasCharacterFaction(person->getCharacterId(), spy::character::FactionEnum::NEUTRAL);
    if (personIsNpc.has_value()) {
        s.nuggetResult = personIsNpc.value();
        if (personIsNpc.value() == 1) {
            // npc --> changes faction
            person->setFaction(character->getFaction());
        } else if (personIsNpc.value() == 0) {
            return {};
        }
    } else {
        s.nuggetResult = std::nullopt;
    }

    return {s};
}