//
// Created by Carolin on 10.06.2020.
//

#include "../ItemChoice_gen.hpp"
#include "evalFunctions_caro.hpp"

std::variant<spy::util::UUID, spy::gadget::GadgetEnum>
ItemChoice_gen::caro(const std::vector<spy::util::UUID> &offeredCharacterIds,
                     const std::vector<spy::gadget::GadgetEnum> &offeredGadgets, const spy::MatchConfig &config,
                     const std::vector<spy::character::CharacterInformation> &characterConfig,
                     const ApproachHelpers &approachHelper) {
    double shift = 0;

    double midCharacterVal = 0;
    spy::util::UUID character;
    double characterVal = -std::numeric_limits<double>::infinity();
    for (const auto &id: offeredCharacterIds) {
        double val = approachHelper.evalFun_caro.value().itemChoice(id, config, characterConfig);
        midCharacterVal += val;
        if (val < shift) {
            shift = val;
        }
        if (val > characterVal) {
            characterVal = val;
            character = id;
        }
    }

    double midGadgetVal = 0;
    spy::gadget::GadgetEnum gadget;
    double gadgetVal = -std::numeric_limits<double>::infinity();
    for (const auto &type: offeredGadgets) {
        double val = approachHelper.evalFun_caro.value().itemChoice(type, config, characterConfig);
        midGadgetVal += val;
        if (val < shift) {
            shift = val;
        }
        if (val > gadgetVal) {
            gadgetVal = val;
            gadget = type;
        }
    }

    double correct = 1;
    if (midCharacterVal != 0 && midGadgetVal != 0) {
        correct = (midCharacterVal - 3 * shift) / (midGadgetVal - 3 * shift); // shift is not positive
    }
    if (characterVal > gadgetVal * correct) {
        return character;
    }
    return gadget;
}