//
// Created by Carolin on 17.05.2020.
//

#ifndef KICLIENT_EQUIPMENTCHOICE_GEN_HPP
#define KICLIENT_EQUIPMENTCHOICE_GEN_HPP

#include <vector>
#include <util/UUID.hpp>
#include <datatypes/gadgets/GadgetEnum.hpp>
#include <datatypes/matchconfig/MatchConfig.hpp>
#include <set>
#include <datatypes/scenario/Scenario.hpp>
#include <datatypes/character/CharacterInformation.hpp>

class EquipmentChoice_gen {
    public:
        EquipmentChoice_gen() = delete;

        static std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>>
        generate(unsigned int difficulty, const std::vector<spy::util::UUID> &chosenCharacterIds,
                 const std::vector<spy::gadget::GadgetEnum> &chosenGadgets, const spy::MatchConfig &config,
                 const spy::scenario::Scenario &scenarioConfig,
                 const std::vector<spy::character::CharacterInformation> &characterConfig);

    private:
        static std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>>
        random(const std::vector<spy::util::UUID> &chosenCharacterIds, const std::vector<spy::gadget::GadgetEnum> &chosenGadgets);
};

#endif //KICLIENT_EQUIPMENTCHOICE_GEN_HPP
