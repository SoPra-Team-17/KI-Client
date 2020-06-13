//
// Created by Carolin on 10.06.2020.
//

#include "State_AI.hpp"
#include "generate/OperationGenerator.hpp"
#include "execute/OperationExecutor.hpp"

namespace spy::gameplay {

    std::vector<State_AI> State_AI::getSuccessorStates(const util::UUID &characterId,
                                                       const spy::MatchConfig &config,
                                                       const libclient::LibClient &libClient) const {
        std::vector<State_AI> successors;
        auto operations = OperationGenerator::generate(*this, characterId, config);

        for (const auto &op: operations) {
            auto successorStates = OperationExecutor::execute(*this, op, config, libClient);
            successors.insert(successors.end(), successorStates.begin(), successorStates.end());
        }

        return successors;
    }

    std::vector<State_AI>
    State_AI::getLeafSuccessorStates(const util::UUID &characterId,
                                     const spy::MatchConfig &config, const libclient::LibClient &libClient) const {
        std::vector<State_AI> leafSuccessors;
        std::vector<State_AI> successors = {*this};

        while (!successors.empty()) {
            // get successors of states in successors list
            std::vector<State_AI> newSuccessors;
            for (const auto &suc: successors) {
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
            hpDiff[id] = damage;
        } else {
            // entry already exists
            hpDiff[id] += damage;
        }

    }
}