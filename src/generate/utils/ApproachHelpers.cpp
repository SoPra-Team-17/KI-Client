//
// Created by Carolin on 19.06.2020.
//

#include "ApproachHelpers.hpp"

ApproachHelpers::ApproachHelpers(unsigned int difficulty,
                                 const spy::MatchConfig &config,
                                 const spy::scenario::Scenario &scenarioConfig,
                                 const std::vector <spy::character::CharacterInformation> &characterConfig) {
    switch (difficulty) {
        case 1:
            // no additional helper needed
            break;
        case 2:
            evalFun_caro = evalFunctions_caro(config, scenarioConfig, characterConfig);
            break;
        default:
            throw std::domain_error("undefined difficulty");
    }

}
