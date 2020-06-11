//
// Created by Carolin on 10.06.2020.
//

#include "evalFunctions_caro.hpp"

double evalFunctions_caro::itemChoice(const std::variant<const spy::util::UUID, const spy::gadget::GadgetEnum> &/*offer*/,
                                      const spy::MatchConfig &/*config*/, const spy::scenario::Scenario &/*scenarioConfig*/,
                                      const std::vector<spy::character::CharacterInformation> &/*characterConfig*/) {
    // TODO implement itemChoice method
    return -1;
}

spy::util::UUID evalFunctions_caro::equipmentChoice(const std::vector<spy::util::UUID> &/*chosenCharacterIds*/,
                                           spy::gadget::GadgetEnum /*chosenGadgetType*/, const spy::MatchConfig &/*config*/,
                                           const spy::scenario::Scenario &/*scenarioConfig*/,
                                           const std::vector<spy::character::CharacterInformation> &/*characterConfig*/) {
    // TODO implement equipmentChoice method
    return {};
}

double evalFunctions_caro::gameOperation(const spy::gameplay::State_AI &/*s*/, const spy::util::UUID &/*characterId*/) {
    // TODO implement gameOperation method
    return -1;
}

