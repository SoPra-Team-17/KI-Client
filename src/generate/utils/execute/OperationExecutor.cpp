//
// Created by Carolin on 11.06.2020.
//

#include "OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::execute(const spy::gameplay::State_AI &s, std::shared_ptr<spy::gameplay::BaseOperation> op,
                           const spy::MatchConfig &config, const libclient::LibClient &libClient) {
    using namespace spy::gameplay;

    switch (op->getType()) {
        case spy::gameplay::OperationEnum::MOVEMENT:
            return OperationExecutor::executeMovement(s, *std::dynamic_pointer_cast<const Movement>(op));

        case OperationEnum::GADGET_ACTION:
            return OperationExecutor::executeGadget(s, *std::dynamic_pointer_cast<const GadgetAction>(op),
                                                    config, libClient);

        case OperationEnum::GAMBLE_ACTION:
            return OperationExecutor::executeGamble(s, *std::dynamic_pointer_cast<const GambleAction>(op));

        case OperationEnum::PROPERTY_ACTION:
            return OperationExecutor::executeProperty(s,
                                                      *std::dynamic_pointer_cast<const PropertyAction>(op),
                                                      config, libClient);

        case OperationEnum::SPY_ACTION:
            return OperationExecutor::executeSpy(s, *std::dynamic_pointer_cast<const SpyAction>(op), config, libClient);

        case OperationEnum::RETIRE:
            return OperationExecutor::executeRetire(s, *std::dynamic_pointer_cast<const RetireAction>(op));

        default:
            throw std::invalid_argument("Execute for this action has not been implemented");
    }
}
