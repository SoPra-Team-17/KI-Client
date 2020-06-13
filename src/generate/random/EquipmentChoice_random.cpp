//
// Created by Carolin on 17.05.2020.
//

#include "../EquipmentChoice_gen.hpp"

std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>>
EquipmentChoice_gen::random(const std::vector<spy::util::UUID> &chosenCharacterIds,
                            const std::vector<spy::gadget::GadgetEnum> &chosenGadgets) {
    return spy::gameplay::EquipmentChoiceGenerator::getRandomEquipmentChoice(chosenCharacterIds, chosenGadgets);
}