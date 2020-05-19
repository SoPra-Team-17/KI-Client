//
// Created by Carolin on 17.05.2020.
//

#ifndef KICLIENT_ITEMCHOICE_HPP
#define KICLIENT_ITEMCHOICE_HPP

#include <vector>
#include <datatypes/gadgets/GadgetEnum.hpp>
#include <util/UUID.hpp>
#include <datatypes/matchconfig/MatchConfig.hpp>
#include <variant>

class ItemChoice {
    public:
        ItemChoice() = delete;

        static auto generate(unsigned int difficulty, std::vector<spy::util::UUID> offeredCharacterIds,
                             std::vector<spy::gadget::GadgetEnum> offeredGadgets,
                             const spy::MatchConfig &config) -> std::variant<spy::util::UUID, spy::gadget::GadgetEnum>;

    private:
        static auto random(std::vector<spy::util::UUID> offeredCharacterIds,
                           std::vector<spy::gadget::GadgetEnum> offeredGadgets) -> std::variant<spy::util::UUID, spy::gadget::GadgetEnum>;
};

#endif //KICLIENT_ITEMCHOICE_HPP
