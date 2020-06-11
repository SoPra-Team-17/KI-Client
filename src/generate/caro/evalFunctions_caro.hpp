//
// Created by Carolin on 10.06.2020.
//

#ifndef KICLIENT_EVALFUNCTIONS_CARO_HPP
#define KICLIENT_EVALFUNCTIONS_CARO_HPP


#include <variant>
#include <datatypes/matchconfig/MatchConfig.hpp>
#include <datatypes/gadgets/GadgetEnum.hpp>
#include <util/UUID.hpp>
#include <datatypes/scenario/Scenario.hpp>
#include <datatypes/character/CharacterInformation.hpp>
#include <generate/utils/State_AI.hpp>

class evalFunctions_caro {
    public:
        evalFunctions_caro() = delete;

        static double itemChoice(const std::variant<const spy::util::UUID, const spy::gadget::GadgetEnum> &offer,
                                       const spy::MatchConfig &config,
                                       const spy::scenario::Scenario &scenarioConfig,
                                       const std::vector <spy::character::CharacterInformation> &characterConfig);

        static spy::util::UUID equipmentChoice(const std::vector<spy::util::UUID> &chosenCharacterIds,
                                      spy::gadget::GadgetEnum chosenGadgetType, const spy::MatchConfig &config,
                                      const spy::scenario::Scenario &scenarioConfig,
                                      const std::vector<spy::character::CharacterInformation> &characterConfig);

        static double gameOperation(const spy::gameplay::State_AI &s, const spy::util::UUID &characterId);
};


#endif //KICLIENT_EVALFUNCTIONS_CARO_HPP
