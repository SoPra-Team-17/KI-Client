//
// Created by Carolin on 11.06.2020.
//

#ifndef KICLIENT_OPERATIONEXECUTOR_HPP
#define KICLIENT_OPERATIONEXECUTOR_HPP


#include <generate/utils/State_AI.hpp>
#include <vector>

class OperationExecutor {
    public:
        OperationExecutor() = delete;

        static std::vector<spy::gameplay::State_AI>
        execute(const spy::gameplay::State_AI &s, std::shared_ptr<spy::gameplay::BaseOperation> op,
                const spy::MatchConfig &config);
};


#endif //KICLIENT_OPERATIONEXECUTOR_HPP
