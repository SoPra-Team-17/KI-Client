//
// Created by Carolin on 10.06.2020.
//

#include "../ItemChoice_gen.hpp"
#include "evalFunctions_caro.hpp"

std::variant<spy::util::UUID, spy::gadget::GadgetEnum>
ItemChoice_gen::caro(const std::vector<spy::util::UUID> &offeredCharacterIds,
                     const std::vector<spy::gadget::GadgetEnum> &offeredGadgets, const spy::MatchConfig &config,
                     const spy::scenario::Scenario &scenarioConfig,
                     const std::vector<spy::character::CharacterInformation> &characterConfig) {
    spy::util::UUID character;
    double characterVal = -1;
    for (const auto &id: offeredCharacterIds) {
        double val = evalFunctions_caro::itemChoice(id, config, scenarioConfig, characterConfig);
        if (val > characterVal) {
            characterVal = val;
            character = id;
        }
    }

    spy::gadget::GadgetEnum gadget;
    double gadgetVal = -1;
    for (const auto &type: offeredGadgets) {
        double val = evalFunctions_caro::itemChoice(type, config, scenarioConfig, characterConfig);
        if (val > gadgetVal) {
            gadgetVal = val;
            gadget = type;
        }
    }

    if (characterVal > gadgetVal) {
        return character;
    }
    return gadget;
}