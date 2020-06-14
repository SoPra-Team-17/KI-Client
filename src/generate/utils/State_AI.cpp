//
// Created by Carolin on 10.06.2020.
//

#include <util/GameLogicUtils.hpp>
#include <gameLogic/validation/ActionValidator.hpp>
#include "State_AI.hpp"
#include "generate/OperationGenerator.hpp"
#include "execute/OperationExecutor.hpp"

namespace spy::gameplay {

    std::vector<State_AI> State_AI::getSuccessorStates(const util::UUID &characterId,
                                                       const spy::MatchConfig &config,
                                                       const libclient::LibClient &libClient) {
        std::vector<State_AI> successors;
        this->wasHoneyTrapUsed = false;
        auto operations = OperationGenerator::generate(*this, characterId, config);

        for (const auto &op: operations) {
            auto successorStates = OperationExecutor::execute(*this, op, config, libClient);
            successors.insert(successors.end(), successorStates.begin(), successorStates.end());
        }

        return successors;
    }

    std::vector<State_AI>
    State_AI::getLeafSuccessorStates(const util::UUID &characterId,
                                     const spy::MatchConfig &config, const libclient::LibClient &libClient) {
        std::vector<State_AI> leafSuccessors;
        std::vector<State_AI> successors = {*this};

        while (!successors.empty()) {
            // get successors of states in successors list
            std::vector<State_AI> newSuccessors;
            for (auto &suc: successors) {
                auto sucBuf = suc.getSuccessorStates(characterId, config, libClient);
                newSuccessors.insert(newSuccessors.end(), sucBuf.begin(), sucBuf.end());
            }

            // erase processed states in successors list and add newSuccessors to the correct list
            successors.clear();
            for (const auto &newSuc: newSuccessors) {
                if (newSuc.isLeafState) {
                    leafSuccessors.push_back(newSuc);
                } else {
                    successors.push_back(newSuc);
                }
            }
        }

        return leafSuccessors;
    }

    void State_AI::addDamage(const spy::character::Character &character, int damage) {
        if (character.hasProperty(character::PropertyEnum::TOUGHNESS)) {
            damage /= 2;
        }

        auto id = character.getCharacterId();
        if (hpDiff.find(id) == hpDiff.end()) {
            // entry does not exist
            hpDiff[id] = -damage;
        } else {
            // entry already exists
            hpDiff[id] -= damage;
        }

    }

    void State_AI::modStateChance(const character::Character &character, double successChance) {
        using spy::character::PropertyEnum;
        using spy::gadget::GadgetEnum;

        // character with clammy clothes only has half the chance of success
        if (character.hasProperty(PropertyEnum::CLAMMY_CLOTHES) ||
            character.hasProperty(PropertyEnum::CONSTANT_CLAMMY_CLOTHES)) {
            successChance /= 2;
        }

        // if char has tradecraft and not mole die, prob. test is repeated
        if (character.hasProperty(PropertyEnum::TRADECRAFT)
            && !character.hasGadget(GadgetEnum::MOLEDIE)) {
            successChance = 1 - std::pow(1 - successChance, 2);
        }

        stateChance *= successChance;
    }

    std::pair<std::vector<State_AI>, bool> State_AI::handleHoneyTrap(const GadgetAction &op, const MatchConfig &config,
                                                                     const libclient::LibClient &libClient) {
        std::vector<State_AI> honeyTrapStates;
        bool foundAlternatives = false;

        auto sourceChar = this->getCharacters().findByUUID(op.getCharacterId());
        auto targetChar = util::GameLogicUtils::findInCharacterSetByCoordinates(this->getCharacters(),
                                                                                op.getTarget());

        if (!this->wasHoneyTrapUsed && config.getHoneyTrapSuccessChance() != 0 && targetChar != this->getCharacters().end()) {
            if (targetChar->hasProperty(spy::character::PropertyEnum::HONEY_TRAP)) {
                auto otherOps = this->getHoneyTrapAlternatives(op, config);
                if (!otherOps.empty()) {
                    foundAlternatives = true;

                    // copy and modify state
                    State_AI s = *this;
                    s.wasHoneyTrapUsed = true;
                    s.modStateChance(*sourceChar, config.getHoneyTrapSuccessChance() / otherOps.size());

                    // reset stuff from executeGadget base method
                    s.operationsLeadingToState.pop_back();
                    s.usedGadgets.pop_back();
                    auto character = s.getCharacters().getByUUID(op.getCharacterId());
                    character->setActionPoints(character->getActionPoints() + 1);
                    s.isLeafState = false;

                    // handle possible states due to honeytrap
                    for (const auto &oOp: otherOps) {
                        auto honeyTrapState = OperationExecutor::executeGadget(s, oOp, config, libClient);
                        honeyTrapStates.insert(honeyTrapStates.end(), honeyTrapState.begin(), honeyTrapState.end());
                    }
                }
            }
        }

        this->modStateChance(*sourceChar, (1 - config.getHoneyTrapSuccessChance()));
        return std::pair<std::vector<State_AI>, bool>(honeyTrapStates,
                                                      foundAlternatives && config.getHoneyTrapSuccessChance() == 1);
    }

    std::vector<GadgetAction>
    State_AI::getHoneyTrapAlternatives(const GadgetAction &op, const MatchConfig &config) const {
        GadgetAction a = op; // keep op const to know regular operation/target

        auto sourceChar = this->getCharacters().findByUUID(a.getCharacterId());
        auto targetChar = util::GameLogicUtils::findInCharacterSetByCoordinates(this->getCharacters(), a.getTarget());

        std::vector<GadgetAction> alternativeTargets;
        for (const auto &character: this->getCharacters()) {
            if (character.getCharacterId() == sourceChar->getCharacterId() ||
                character.getCharacterId() == targetChar->getCharacterId() ||
                !character.getCoordinates().has_value()) {
                // alternative action target is source or target or not in map
                continue;
            }

            auto otherTarget = character.getCoordinates().value();
            if (this->getMap().isInside(otherTarget)) {
                a.setTarget(otherTarget);
                if (gameplay::ActionValidator::validate(*this, std::make_shared<GadgetAction>(a), config)) {
                    alternativeTargets.push_back(GadgetAction(a));
                }
            }
        }

        return alternativeTargets;
    }

    bool State_AI::handleBabysitter(const GadgetAction &op, const MatchConfig &config) {
        if (config.getBabysitterSuccessChance() != 0) {
            auto babysitter = util::GameLogicUtils::babysitterNumber(*this, std::make_shared<GadgetAction>(op));
            if (babysitter > 0) {
                this->stateChance *= std::pow(1 - config.getBabysitterSuccessChance(), babysitter);
                if (config.getBabysitterSuccessChance() == 1) {
                    return true;
                }
            }
        }
        return false;
    }
}