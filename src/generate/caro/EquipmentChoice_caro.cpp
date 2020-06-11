//
// Created by Carolin on 10.06.2020.
//

#include "../EquipmentChoice_gen.hpp"
#include "evalFunctions_caro.hpp"

std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>>
EquipmentChoice_gen::caro(const std::vector<spy::util::UUID> &chosenCharacterIds,
                          const std::vector<spy::gadget::GadgetEnum> &chosenGadgets, const spy::MatchConfig &config,
                          const spy::scenario::Scenario &scenarioConfig,
                          const std::vector<spy::character::CharacterInformation> &characterConfig) {

    std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>> equipment;
    for (auto id: chosenCharacterIds) {
        equipment[id];  //add all keys to map
    }

    for (const auto &type: chosenGadgets) {
        equipment[evalFunctions_caro::equipmentChoice(chosenCharacterIds, type, config, scenarioConfig, characterConfig)].insert(type);
    }

    return equipment;
}
