//
// Created by Carolin on 13.06.2020.
//

#include <util/GameLogicUtils.hpp>
#include "../OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeMoleDie(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                                  const spy::MatchConfig &config, const libclient::LibClient &libClient) {
    std::vector<spy::gameplay::State_AI> honeyStates;
    std::vector<spy::gameplay::State_AI> myStates;
    spy::gameplay::State_AI my = state;

    // character that issues the action and remove mole die from their inventory
    auto character = my.getCharacters().getByUUID(op.getCharacterId());
    character->removeGadget(op.getGadget());


    // check if target field has character
    auto person = spy::util::GameLogicUtils::getInCharacterSetByCoordinates(my.getCharacters(), op.getTarget());
    if (person != my.getCharacters().end()) {
        // honey trap
        auto honeyTrapResult = my.handleHoneyTrap(op, config, libClient);
        honeyStates = honeyTrapResult.first;
        if (honeyTrapResult.second) {
            return honeyStates;
        }

        // mole die goes into the person inventory
        auto targetPerson = spy::util::GameLogicUtils::getInCharacterSetByCoordinates(my.getCharacters(), op.getTarget());
        targetPerson->addGadget(std::make_shared<spy::gadget::Gadget>(spy::gadget::GadgetEnum::MOLEDIE));

        honeyStates.push_back(my);
        return honeyStates;
    }

    // mole die bounces into the inventory of the closest person
    auto closestPoints = spy::util::GameLogicUtils::getCharacterNearFields(my, op.getTarget());
    for (const auto &p: closestPoints) {
        spy::gameplay::State_AI buf = my;
        buf.stateChance *= 1 / closestPoints.size();
        auto closestPerson = spy::util::GameLogicUtils::getInCharacterSetByCoordinates(buf.getCharacters(), p);
        closestPerson->addGadget(std::make_shared<spy::gadget::Gadget>(spy::gadget::GadgetEnum::MOLEDIE));
        myStates.push_back(buf);
    }

    honeyStates.insert(honeyStates.end(), myStates.begin(), myStates.end());
    return honeyStates;



}