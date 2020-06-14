//
// Created by Carolin on 13.06.2020.
//

#include <util/GameLogicUtils.hpp>
#include "../OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeRocketPen(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                                    const spy::MatchConfig &config, const libclient::LibClient &libClient) {
    std::vector<spy::gameplay::State_AI> honeyStates;
    spy::gameplay::State_AI myState = state;

    // honey trap and babysitter
    auto honeyTrapResult = myState.handleHoneyTrap(op, config, libClient);
    honeyStates = honeyTrapResult.first;
    if (honeyTrapResult.second) {
        return honeyStates;
    }
    if (myState.handleBabysitter(op, config)) {
        return {};
    }

    // destroy potential wall on target field
    bool targetHasWall = (myState.getMap().getField(op.getTarget()).getFieldState() == spy::scenario::FieldStateEnum::WALL);
    if (targetHasWall) {
        myState.getMap().getField(op.getTarget()).setFieldState(spy::scenario::FieldStateEnum::FREE);
        myState.destroyedWalls.push_back(op.getTarget());
    }

    //Neighboring fields that have walls on them
    auto fieldWithWalls = spy::util::GameLogicUtils::getNearFieldsInDist(myState, op.getTarget(), 1,
                                                                    [&myState](const spy::util::Point &p) {
                                                                        return myState.getMap().getField(
                                                                                p).getFieldState() ==
                                                                                spy::scenario::FieldStateEnum::WALL;
                                                                    });
    if (fieldWithWalls.second) {
        for (const auto &p : fieldWithWalls.first) {
            myState.getMap().getField(p).setFieldState(spy::scenario::FieldStateEnum::FREE);
            myState.destroyedWalls.push_back(p);
        }
    }

    auto damage = config.getRocketPenDamage();

    // damage on targetfield, if there is a person
    bool targetHasPerson = spy::util::GameLogicUtils::isPersonOnField(myState, op.getTarget());
    if (targetHasPerson) {
        auto person = spy::util::GameLogicUtils::getInCharacterSetByCoordinates(myState.getCharacters(), op.getTarget());
        spy::util::GameLogicUtils::applyDamageToCharacter(myState, *person, damage);
        myState.addDamage(*person, damage);
    }

    // get characters on neighboring fields
    auto charPoints = spy::util::GameLogicUtils::getNearFieldsInDist(myState, op.getTarget(), 1, [&myState](const spy::util::Point &p) {
        return spy::util::GameLogicUtils::isPersonOnField(myState, p);
    });
    if (charPoints.second) {
        for (const auto &p : charPoints.first) {
            auto person = spy::util::GameLogicUtils::getInCharacterSetByCoordinates(myState.getCharacters(), p);
            spy::util::GameLogicUtils::applyDamageToCharacter(myState, *person, damage);
            myState.addDamage(*person, damage);
        }
    }

    // remove rocket pen from inventory
    auto character = myState.getCharacters().getByUUID(op.getCharacterId());
    character->removeGadget(spy::gadget::GadgetEnum::ROCKET_PEN);

    honeyStates.push_back(myState);
    return honeyStates;
}