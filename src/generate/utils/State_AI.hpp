//
// Created by Carolin on 10.06.2020.
//

#ifndef KICLIENT_STATE_AI_HPP
#define KICLIENT_STATE_AI_HPP

#include <datatypes/gameplay/State.hpp>
#include <utility>
#include <datatypes/matchconfig/MatchConfig.hpp>
#include <LibClient.hpp>

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
            bool wasHoneyTrapUsed = false; // reset to false for each new operation execution
            double stateChance = 1;
            std::vector<std::shared_ptr<BaseOperation>> operationsLeadingToState;
            bool isLeafState = false;
            std::vector<spy::gadget::GadgetEnum> collectedGadgets;
            std::vector<spy::gadget::GadgetEnum> grappledGadgets;
            std::vector<spy::gadget::GadgetEnum> usedGadgets;
            int chipDiff = 0;
            std::set<spy::util::UUID> removedClammyClothes;
            std::map<spy::util::UUID, int> hpDiff;
            std::vector<std::optional<double>> observationResult; // if nullopt modify this value (no info about pocket litter)
            int unknownGadgetsModifyingSuccess; // for each gadget modify stateChance

            [[nodiscard]] std::vector<State_AI> getLeafSuccessorStates(const spy::util::UUID &characterId,
                                                                       const spy::MatchConfig &config,
                                                                       const libclient::LibClient &libClient);

            /**
             * adds hp damage to given character (modifies hpDiff list)
             * @param character character whose hp changed
             * @param damage hp damage (-> is getting subtracted, negative damage equals receiving hp)
             */
            void addDamage(const spy::character::Character &character, int damage);

            /**
             * modifies state chance according to probability test with character
             * @param character character that does chance test
             * @param successChance chance of success for test
             */
            void modStateChance(const spy::character::Character &character, double successChance);

            /**
             * handles honey trap for operation (modifies stateChance)
             * @param op action to execute
             * @param config current match config
             * @param libClient object of libClient
             * @return pair of
             *               std::vector<State_AI containing all executed states due to honeytrap property
             *               bool that is true if op will not take place, else false
             */
            [[nodiscard]] std::pair<std::vector<State_AI>, bool>
            handleHoneyTrap(const GadgetAction &op, const MatchConfig &config, const libclient::LibClient &libClient);

            /**
             * handles babysitter for operation (modifies stateChance)
             * @param op action to execute
             * @param config current match config
             * @return true if op will not take place, else false
             */
            bool handleBabysitter(const GadgetAction &op, const MatchConfig &config);

        private:
            [[nodiscard]] std::vector<State_AI> getSuccessorStates(const spy::util::UUID &characterId,
                                                                   const spy::MatchConfig &config,
                                                                   const libclient::LibClient &libClient);

            std::vector<GadgetAction>
            getHoneyTrapAlternatives(const GadgetAction &op, const MatchConfig &config) const;
    };
}


#endif //KICLIENT_STATE_AI_HPP
