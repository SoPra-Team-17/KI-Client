//
// Created by Carolin on 10.06.2020.
//

#ifndef KICLIENT_STATE_AI_HPP
#define KICLIENT_STATE_AI_HPP

#include <datatypes/gameplay/State.hpp>
#include <utility>
#include <datatypes/matchconfig/MatchConfig.hpp>

namespace spy::gameplay {
    class State_AI : public State {
        public:

            State_AI() : State() {};

            State_AI(unsigned int currentRound, scenario::FieldMap map, std::set<int> mySafeCombinations,
                     character::CharacterSet characters, const std::optional<util::Point> &catCoordinates,
                     const std::optional<util::Point> &janitorCoordinates) : State(currentRound, std::move(map),
                                                                                   std::move(mySafeCombinations),
                                                                                   std::move(characters),
                                                                                   catCoordinates,
                                                                                   janitorCoordinates) {};

            explicit State_AI(const State &s) : State(s) {};

            // TODO: (evaluation) variables: have to be set by execute methods
            double stateChance = 1;
            std::vector<std::shared_ptr<spy::gameplay::BaseOperation>> operationsLeadingToState;
            bool isLeafState = false;
            std::set<spy::gadget::GadgetEnum> collectedGadgets;
            std::set<spy::gadget::GadgetEnum> grappledGadgets;
            std::set<spy::gadget::GadgetEnum> usedGadgets;
            int chipDiff = 0;
            std::set<spy::util::UUID> removedClammyClothes;
            std::map<spy::util::UUID, int> hpDiff;


            [[nodiscard]] std::vector<State_AI> getLeafSuccessorStates(const spy::util::UUID &characterId,
                                                         const spy::MatchConfig &config) const;

            void addDamage(const spy::character::Character &character, int damage);

        private:
            [[nodiscard]] std::vector<State_AI> getSuccessorStates(const spy::util::UUID &characterId,
                                                     const spy::MatchConfig &config) const;
    };
}


#endif //KICLIENT_STATE_AI_HPP
