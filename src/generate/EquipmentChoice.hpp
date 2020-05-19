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

        static auto generate(unsigned int difficulty, std::vector<spy::util::UUID> chosenCharacterIds,
                             std::vector<spy::gadget::GadgetEnum> chosenGadgets,
                             const spy::MatchConfig &config) -> std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>>;

    private:
        static auto random(std::vector<spy::util::UUID> chosenCharacterIds,
                           std::vector<spy::gadget::GadgetEnum> chosenGadgets) -> std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>>;
};

#endif //KICLIENT_EQUIPMENTCHOICE_HPP
