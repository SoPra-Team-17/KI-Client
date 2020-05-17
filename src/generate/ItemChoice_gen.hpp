//
// Created by Carolin on 17.05.2020.
//

#ifndef KICLIENT_ITEMCHOICE_GEN_HPP
#define KICLIENT_ITEMCHOICE_GEN_HPP

#include <vector>
#include <datatypes/gadgets/GadgetEnum.hpp>
#include <util/UUID.hpp>
#include <datatypes/matchconfig/MatchConfig.hpp>
#include <variant>
#include <datatypes/scenario/Scenario.hpp>
#include <datatypes/character/CharacterInformation.hpp>

class ItemChoice_gen {
    public:
        ItemChoice_gen() = delete;

        static std::variant<spy::util::UUID, spy::gadget::GadgetEnum>
        generate(unsigned int difficulty, const std::vector<spy::util::UUID> &offeredCharacterIds,
                 const std::vector<spy::gadget::GadgetEnum> &offeredGadgets, const spy::MatchConfig &config,
                 const spy::scenario::Scenario &scenarioConfig,
                 const std::vector<spy::character::CharacterInformation> &characterConfig);

    private:
        static std::variant<spy::util::UUID, spy::gadget::GadgetEnum>
        random(const std::vector<spy::util::UUID> &offeredCharacterIds, const std::vector<spy::gadget::GadgetEnum> &offeredGadgets);
};

#endif //KICLIENT_ITEMCHOICE_GEN_HPP
