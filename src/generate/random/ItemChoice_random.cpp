//
// Created by Carolin on 17.05.2020.
//

#include "../ItemChoice_gen.hpp"
#include <gameLogic/generation/ItemChoiceGenerator.hpp>
#include <util/GameLogicUtils.hpp>

std::variant<spy::util::UUID, spy::gadget::GadgetEnum>
ItemChoice_gen::random(const std::vector<spy::util::UUID> &offeredCharacterIds,
                       const std::vector<spy::gadget::GadgetEnum> &offeredGadgets) {
    if (!offeredCharacterIds.empty() && !offeredGadgets.empty()) {
        auto character = spy::util::GameLogicUtils::getRandomItemFromContainer(offeredCharacterIds);
        auto gadget = spy::util::GameLogicUtils::getRandomItemFromContainer(offeredGadgets);
        auto r = spy::util::GameLogicUtils::probabilityTest(0.5);
        if (r) {
            return std::variant<spy::util::UUID, spy::gadget::GadgetEnum>(*character);
        } else {
            return std::variant<spy::util::UUID, spy::gadget::GadgetEnum>(*gadget);
        }
    }
    if (offeredCharacterIds.empty()) {
        return std::variant<spy::util::UUID, spy::gadget::GadgetEnum>(
                *spy::util::GameLogicUtils::getRandomItemFromContainer(offeredGadgets));
    }
    if (offeredGadgets.empty()) {
        return std::variant<spy::util::UUID, spy::gadget::GadgetEnum>(
                *spy::util::GameLogicUtils::getRandomItemFromContainer(offeredCharacterIds));
    }

    throw std::domain_error("offeredCharacterIds and offeredGadgets were empty");
}