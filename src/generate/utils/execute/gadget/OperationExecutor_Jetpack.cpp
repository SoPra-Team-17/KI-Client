//
// Created by Carolin on 13.06.2020.
//

#include <util/GameLogicUtils.hpp>
#include "../OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeJetpack(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op) {
    spy::gameplay::State_AI s = state;

    auto character = s.getCharacters().getByUUID(op.getCharacterId());
    if (spy::gameplay::Movement::getMoveDistance(character->getCoordinates().value(), op.getTarget()) <=
        character->getMovePoints()) {
        return {}; // you can move there
    }

    auto neighbourPoints =
        spy::util::GameLogicUtils::getNearFieldsInDist(s, op.getTarget(), 1,
                                                       [&s](const spy::util::Point &p) {
                                                           auto fieldState = s.getMap().getField(p).getFieldState();
                                                           return (fieldState != spy::scenario::FieldStateEnum::FREE &&
                                                                   fieldState != spy::scenario::FieldStateEnum::WALL) ||
                                                                  spy::util::GameLogicUtils::isPersonOnField(s, p) ||
                                                                  s.getCatCoordinates() == p;
                                                       });
    if (neighbourPoints.first.empty()) {
        if (character->hasProperty(spy::character::PropertyEnum::FLAPS_AND_SEALS)) {
            auto morePoints =
                spy::util::GameLogicUtils::getNearFieldsInDist(s, op.getTarget(), 1,
                                                               [&s](const spy::util::Point &p) {
                                                                   return s.getMap().getField(p).getFieldState() ==
                                                                          spy::scenario::FieldStateEnum::SAFE;
                                                               });
            if (morePoints.first.empty()) {
                return {}; // no obvious reason to fly there
            }
        }
        return {}; // no obvious reason to fly there
    }

    character->setCoordinates(op.getTarget());
    character->removeGadget(op.getGadget());

    return {s};
}