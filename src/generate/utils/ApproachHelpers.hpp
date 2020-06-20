//
// Created by Carolin on 19.06.2020.
//

#ifndef KICLIENT_APPROACHHELPERS_HPP
#define KICLIENT_APPROACHHELPERS_HPP


#include <datatypes/scenario/Scenario.hpp>
#include <datatypes/matchconfig/MatchConfig.hpp>
#include <datatypes/character/CharacterInformation.hpp>
#include <generate/caro/evalFunctions_caro.hpp>

class ApproachHelpers {
    public:
        ApproachHelpers() = default;

        ApproachHelpers(unsigned int difficulty,
                        const spy::MatchConfig &config,
                        const spy::scenario::Scenario &scenarioConfig,
                        const std::vector <spy::character::CharacterInformation> &characterConfig);

        std::optional<evalFunctions_caro> evalFun_caro;

};


#endif //KICLIENT_APPROACHHELPERS_HPP
