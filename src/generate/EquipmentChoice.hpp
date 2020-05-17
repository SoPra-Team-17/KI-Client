//
// Created by Carolin on 17.05.2020.
//

#ifndef KICLIENT_EQUIPMENTCHOICE_HPP
#define KICLIENT_EQUIPMENTCHOICE_HPP

#include <vector>
#include <util/UUID.hpp>
#include <datatypes/gadgets/GadgetEnum.hpp>
#include <datatypes/matchconfig/MatchConfig.hpp>
#include <set>

class EquipmentChoice {
    public:
        EquipmentChoice() = delete;

        static std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>> generate(unsigned int difficulty, std::vector<spy::util::UUID> chosenCharacterIds,
                 std::vector<spy::gadget::GadgetEnum> chosenGadgets, const spy::MatchConfig &config);

    private:
        static std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>> random(std::vector<spy::util::UUID> chosenCharacterIds, std::vector<spy::gadget::GadgetEnum> chosenGadgets);
};

#endif //KICLIENT_EQUIPMENTCHOICE_HPP
