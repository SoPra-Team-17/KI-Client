//
// Created by Carolin on 17.05.2020.
//

#include "../ItemChoice_gen.hpp"
#include <gameLogic/generation/ItemChoiceGenerator.hpp>

std::variant<spy::util::UUID, spy::gadget::GadgetEnum>
ItemChoice_gen::random(const std::vector<spy::util::UUID> &offeredCharacterIds,
                       const std::vector<spy::gadget::GadgetEnum> &offeredGadgets) {
    return spy::gameplay::ItemChoiceGenerator::getRandomItemChoice(offeredCharacterIds, offeredGadgets);
}