//
// Created by Carolin on 17.05.2020.
//

#include "EquipmentChoice_gen.hpp"

#include <utility>



std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>>
EquipmentChoice_gen::generate(unsigned int difficulty, const std::vector<spy::util::UUID> &chosenCharacterIds,
                              const std::vector<spy::gadget::GadgetEnum> &chosenGadgets, const spy::MatchConfig &/*config*/,
                              const spy::scenario::Scenario &/*scenarioConfig*/,
                              const std::vector<spy::character::CharacterInformation> &/*characterConfig*/) {
    switch(difficulty) {
        case 1:
            return random(std::move(chosenCharacterIds), std::move(chosenGadgets));
        default:
            throw std::domain_error("undefined difficulty");
    }
}
