//
// Created by Carolin on 17.05.2020.
//

#include "ItemChoice.hpp"

std::variant<spy::util::UUID, spy::gadget::GadgetEnum>
ItemChoice::generate(unsigned int difficulty, std::vector<spy::util::UUID> offeredCharacterIds,
                     std::vector<spy::gadget::GadgetEnum> offeredGadgets, const spy::MatchConfig &/*config*/) {
    switch (difficulty) {
        case 1:
            return random(offeredCharacterIds, offeredGadgets);
        default:
            throw std::domain_error("undefined difficulty");
    }
}
