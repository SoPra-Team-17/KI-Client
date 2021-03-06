//
// Created by Carolin on 10.06.2020.
//

#include <util/GameLogicUtils.hpp>
#include <gameLogic/validation/ActionValidator.hpp>
#include "State_AI.hpp"
#include "generate/OperationGenerator.hpp"
#include "execute/OperationExecutor.hpp"
#include <spdlog/spdlog.h>

namespace spy::gameplay {

    void State_AI::getSuccessorStates(const util::UUID &characterId,
                                      const spy::MatchConfig &config,
                                      const libclient::LibClient &libClient,
                                      std::vector<State_AI> &successors) {
        this->wasHoneyTrapUsed = false;
        auto operations = OperationGenerator::generate(*this, characterId, config);

        for (const auto &op: operations) {
            auto successorStates = OperationExecutor::execute(*this, op, config, libClient);
            successors.insert(successors.end(), successorStates.begin(), successorStates.end());
        }
    }

    std::vector<State_AI>
    State_AI::getLeafSuccessorStates(const util::UUID &characterId,
                                     const spy::MatchConfig &config, const libclient::LibClient &libClient,
                                     std::optional<std::chrono::milliseconds> maxDur) {
        auto endClock = std::chrono::system_clock::now();
        if (maxDur.has_value()) {
            endClock += maxDur.value();
        } else {
            endClock += std::chrono::hours(std::numeric_limits<int>::max());
        }

        bool earlyReturn;
        std::vector<State_AI> leafSuccessors;
        std::vector<State_AI> successors = {*this};

        while (!successors.empty()) {
            // get successors of states in successors list
            std::vector<State_AI> newSuccessors;
            auto suc = successors.back();
            suc.getSuccessorStates(characterId, config, libClient, newSuccessors);
            successors.pop_back();

            // early return
            earlyReturn = std::chrono::high_resolution_clock::now() > endClock;

            // erase processed states in successors list and add newSuccessors to the correct list if they are no duplicate
            for (const auto &newSuc: newSuccessors) {
                if (newSuc.isLeafState) {
                    if (!newSuc.isDuplicate(leafSuccessors, characterId)) {
                        leafSuccessors.push_back(newSuc);
                    }
                } else if (!earlyReturn && !newSuc.isDuplicate(successors, characterId)) {
                    // if earlyReturn we do not have to update this list
                    successors.push_back(newSuc);
                }
            }

            if (earlyReturn) {
                // exit while loop and return leafSuccessors
                break;
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
        auto chance = getChanceForCharacter(character, successChance);

        stateChance *= chance;
    }

    std::pair<std::vector<State_AI>, bool> State_AI::handleHoneyTrap(const GadgetAction &op, const MatchConfig &config,
                                                                     const libclient::LibClient &libClient) {
        std::vector<State_AI> honeyTrapStates;
        bool foundAlternatives = false;

        auto sourceChar = this->getCharacters().findByUUID(op.getCharacterId());
        auto targetChar = util::GameLogicUtils::findInCharacterSetByCoordinates(this->getCharacters(),
                                                                                op.getTarget());

        if (!this->wasHoneyTrapUsed && config.getHoneyTrapSuccessChance() != 0 &&
            targetChar != this->getCharacters().end()) {
            if (targetChar->hasProperty(spy::character::PropertyEnum::HONEY_TRAP)) {
                auto otherOps = this->getHoneyTrapAlternatives(op, config);
                if (!otherOps.empty()) {
                    foundAlternatives = true;

                    // copy and modify state
                    State_AI s = *this;
                    s.wasHoneyTrapUsed = true;
                    s.modStateChance(*sourceChar,
                                     config.getHoneyTrapSuccessChance() / static_cast<double>(otherOps.size()));

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
    State_AI::getHoneyTrapAlternatives(GadgetAction a, const MatchConfig &config) const {
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
                    alternativeTargets.emplace_back(a);
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

    bool State_AI::operator==(const State_AI &rhs) const {
        bool isSame = std::tie(stateChance, isLeafState, collectedGadgets, usedGadgets, chipDiff, removedClammyClothes,
                               addedClammyClothes, hpDiff, observationResult, nuggetResult, chickenfeedResult,
                               mowResult,
                               unknownGadgetsModifyingSuccess,
                               removedCocktails, poisonedCocktails, destroyedRoulettes, invertedRoulette, foggyFields,
                               destroyedWalls, spyResult, spyedSafes, movedMoledieTo) ==
                      std::tie(rhs.stateChance, rhs.isLeafState, rhs.collectedGadgets, rhs.usedGadgets, rhs.chipDiff,
                               rhs.removedClammyClothes, rhs.addedClammyClothes, rhs.hpDiff, rhs.observationResult,
                               rhs.nuggetResult, rhs.chickenfeedResult, rhs.mowResult,
                               rhs.unknownGadgetsModifyingSuccess,
                               rhs.removedCocktails, rhs.poisonedCocktails, rhs.destroyedRoulettes,
                               rhs.invertedRoulette,
                               rhs.foggyFields, rhs.destroyedWalls, rhs.spyResult, rhs.spyedSafes, rhs.movedMoledieTo);

        if (isSame && operationsLeadingToState.size() == rhs.operationsLeadingToState.size()) {
            for (auto i = 0U; i < operationsLeadingToState.size(); i++) {
                auto opS = operationsLeadingToState[i];
                auto opRhs = rhs.operationsLeadingToState[i];
                if (opS->getType() != opRhs->getType()) {
                    return false;
                }
                if (opS->getType() == spy::gameplay::OperationEnum::GADGET_ACTION) {
                    auto gadS = std::dynamic_pointer_cast<const GadgetAction>(opS);
                    auto gadRhs = std::dynamic_pointer_cast<const GadgetAction>(opRhs);
                    if (gadS->getGadget() != gadRhs->getGadget()) {
                        return false;
                    }
                }
            }
            return true;
        }

        return false;
    }

    bool State_AI::isDuplicate(const std::vector<State_AI> &list, const util::UUID &charId) const {
        const auto &state = *this;
        auto it = std::find_if(list.begin(), list.end(), [&state, &charId](const State_AI &s) {
            if (s == state) {
                auto charS = s.getCharacters().findByUUID(charId)->getCoordinates().value();
                auto charState = state.getCharacters().findByUUID(charId)->getCoordinates().value();
                return charS == charState;
            }
            return false;
        });
        return it != list.end();
    }

    double State_AI::getGambleWinningChance(const spy::character::Character &character, const spy::scenario::Field &field) {
        double winningChance = 18.0/37.0;
        if (character.hasProperty(spy::character::PropertyEnum::LUCKY_DEVIL)) {
            winningChance = 32.0/37.0;
        } else if (character.hasProperty(spy::character::PropertyEnum::JINX)) {
            winningChance = 13.0/37.0;
        }
        winningChance = field.isInverted().value() ? (1-winningChance) : winningChance;

        return getChanceForCharacter(character, winningChance);
    }

    double State_AI::getChanceForCharacter(const spy::character::Character &character, double chance) {
        using spy::character::PropertyEnum;
        using spy::gadget::GadgetEnum;

        // character with clammy clothes only has half the chance of success
        if (character.hasProperty(PropertyEnum::CLAMMY_CLOTHES) ||
            character.hasProperty(PropertyEnum::CONSTANT_CLAMMY_CLOTHES)) {
            chance /= 2;
        }

        // if char has tradecraft and not mole die, prob. test is repeated
        if (character.hasProperty(PropertyEnum::TRADECRAFT)
            && !character.hasGadget(GadgetEnum::MOLEDIE)) {
            chance = 1 - std::pow(1 - chance, 2);
        }

        return chance;
    }

}