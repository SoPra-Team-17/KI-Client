//
// Created by Carolin on 17.05.2020.
//

#include "EquipmentChoice.hpp"

std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>>
EquipmentChoice::generate(unsigned int difficulty, const std::vector<spy::util::UUID> &chosenCharacterIds,
                          const std::vector<spy::gadget::GadgetEnum> &chosenGadgets,
                          const spy::MatchConfig &/*config*/) {
    switch (difficulty) {
        case 1:
            return random(chosenCharacterIds, chosenGadgets);
        default:
            throw std::domain_error("undefined difficulty");
    }
}
