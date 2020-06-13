//
// Created by Carolin on 13.06.2020.
//

#include <util/GameLogicUtils.hpp>
#include "../OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeMothballPouch(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                                        const spy::MatchConfig &config) {
    spy::gameplay::State_AI s = state;

    // get persons neighboring the fireplace
    auto charPoints = spy::util::GameLogicUtils::getNearFieldsInDist(s, op.getTarget(), 1, [&s](const spy::util::Point &p) {
        return spy::util::GameLogicUtils::isPersonOnField(s, p);
    });

    if (charPoints.second) {
        for (const auto &p : charPoints.first) {
            auto person = spy::util::GameLogicUtils::getInCharacterSetByCoordinates(s.getCharacters(), p);
            spy::util::GameLogicUtils::applyDamageToCharacter(s, *person, config.getMothballPouchDamage());
            s.addDamage(*person, config.getMothballPouchDamage());
        }
    }

    // subtract a usage
    auto character = s.getCharacters().getByUUID(op.getCharacterId());
    auto gadget = character->getGadget(spy::gadget::GadgetEnum::MOTHBALL_POUCH).value();
    gadget->setUsagesLeft(gadget->getUsagesLeft().value() - 1);

    // remove gadget from inventory if no usages left!
    if (gadget->getUsagesLeft() == 0) {
        character->removeGadget(spy::gadget::GadgetEnum::MOTHBALL_POUCH);
    }

    return {s};
}