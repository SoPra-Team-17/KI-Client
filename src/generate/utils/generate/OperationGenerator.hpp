//
// Created by Carolin on 11.06.2020.
//

#ifndef KICLIENT_OPERATIONGENERATOR_HPP
#define KICLIENT_OPERATIONGENERATOR_HPP


#include <datatypes/gameplay/BaseOperation.hpp>
#include <vector>
#include <generate/utils/State_AI.hpp>

class OperationGenerator {
    public:
        OperationGenerator() = delete;

        static auto generate(const spy::gameplay::State_AI &s, const spy::util::UUID &characterId,
                             const spy::MatchConfig &config) -> std::vector<std::shared_ptr<spy::gameplay::BaseOperation>>;
};


#endif //KICLIENT_OPERATIONGENERATOR_HPP
