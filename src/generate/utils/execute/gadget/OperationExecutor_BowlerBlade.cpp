//
// Created by Carolin on 13.06.2020.
//

#include <util/GameLogicUtils.hpp>
#include "../OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeBowlerBlade(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                                      const spy::MatchConfig &config, const libclient::LibClient &libClient) {
    if (config.getBowlerBladeHitChance() == 0 || config.getBowlerBladeDamage() == 0) {
        return {};
    }

    std::vector<spy::gameplay::State_AI> honeyStates;
    std::vector<spy::gameplay::State_AI> myStates;
    spy::gameplay::State_AI sSuccess = state;

    auto character = sSuccess.getCharacters().getByUUID(op.getCharacterId());
    character->removeGadget(op.getGadget());

    sSuccess.modStateChance(*character, config.getBowlerBladeHitChance());


    // honey trap and babysitter
    auto honeyTrapResult = sSuccess.handleHoneyTrap(op, config, libClient);
    honeyStates = honeyTrapResult.first;
    if (honeyTrapResult.second) {
        return honeyStates;
    }
    if (sSuccess.handleBabysitter(op, config)) {
        return {};
    }

    // magnetic watch
    auto targetPerson = spy::util::GameLogicUtils::getInCharacterSetByCoordinates(sSuccess.getCharacters(),
                                                                                  op.getTarget());
    auto hasTargetMagneticWatch = libClient.hasCharacterGadget(targetPerson->getCharacterId(),
                                                               spy::gadget::GadgetEnum::MAGNETIC_WATCH);
    if (hasTargetMagneticWatch.value()) {
        if (hasTargetMagneticWatch.value() == 1) {
            return honeyStates;
        } else {
            sSuccess.stateChance *= (1 - hasTargetMagneticWatch.value());
        }
    } else {
        sSuccess.unknownGadgetsModifyingSuccess++;
    }

    // successful usage of bowler blade
    auto damage = config.getBowlerBladeDamage();
    spy::util::GameLogicUtils::applyDamageToCharacter(sSuccess, *targetPerson, damage);
    sSuccess.addDamage(*targetPerson, damage);

    // move hat to random free field of target person
    auto newHatPoints = spy::util::GameLogicUtils::getNearFields(state, op.getTarget(), [&state](const spy::util::Point &pt) {
        bool fieldHasGadget = state.getMap().getField(pt).getGadget().has_value();
        bool fieldHasPerson = spy::util::GameLogicUtils::isPersonOnField(state, pt);
        bool isFieldFree = state.getMap().getField(pt).getFieldState() == spy::scenario::FieldStateEnum::FREE;
        return !fieldHasGadget && !fieldHasPerson && isFieldFree;
    });
    for (const auto &p: newHatPoints) {
        spy::gameplay::State_AI buf = sSuccess;
        buf.stateChance *= 1 / newHatPoints.size();
        buf.getMap().getField(p).setGadget(character->getGadget(spy::gadget::GadgetEnum::BOWLER_BLADE));
        myStates.push_back(buf);
    }

    honeyStates.insert(honeyStates.end(), myStates.begin(), myStates.end());
    return honeyStates;
}