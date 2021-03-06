//
// Created by Carolin on 17.05.2020.
//

#include "ItemChoice_gen.hpp"


std::variant<spy::util::UUID, spy::gadget::GadgetEnum>
ItemChoice_gen::generate(unsigned int difficulty, const std::vector<spy::util::UUID> &offeredCharacterIds,
                         const std::vector<spy::gadget::GadgetEnum> &offeredGadgets, const spy::MatchConfig &config,
                         const std::vector<spy::character::CharacterInformation> &characterConfig,
                         const ApproachHelpers &approachHelper) {
    switch (difficulty) {
        case 1:
            return random(offeredCharacterIds, offeredGadgets);
        case 2:
            return caro(offeredCharacterIds, offeredGadgets, config, characterConfig, approachHelper);
        default:
            throw std::domain_error("undefined difficulty");
    }
}
