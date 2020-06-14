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

            explicit State_AI(const State &s, const spy::util::UUID characterId) : State(
                    s) { startPoint = this->getCharacters().findByUUID(characterId)->getCoordinates().value(); };

            bool wasHoneyTrapUsed = false; // reset to false for each new operation execution
            double stateChance = 1;
            std::vector<std::shared_ptr<BaseOperation>> operationsLeadingToState;
            bool isLeafState = false;
            spy::util::Point startPoint;
            std::vector<spy::gadget::GadgetEnum> collectedGadgets;
            std::vector<spy::gadget::GadgetEnum> usedGadgets;
            int chipDiff = 0;
            std::set<spy::util::UUID> removedClammyClothes;
            std::set<spy::util::UUID> addedClammyClothes;
            std::map<spy::util::UUID, int> hpDiff;
            std::vector<std::optional<double>> observationResult; // if nullopt modify this value (no info about pocket litter)
            std::optional<double> nuggetResult; // if nullopt modify this value (no info about faction)
            std::pair<std::optional<double>, std::optional<spy::util::UUID>> chickenfeedResult; // if nullopt modify this value (no info about faction)
            std::pair<int, std::optional<spy::util::UUID>> mowResult;
            int unknownGadgetsModifyingSuccess; // for each gadget modify stateChance
            std::vector<std::pair<std::variant<spy::util::UUID, spy::util::Point>, bool>> removedCocktails;
            std::vector<std::variant<spy::util::UUID, spy::util::Point>> poisonedCocktails;
            std::vector<spy::util::Point> destroyedRoulettes;
            spy::util::Point invertedRoulette;
            std::vector<spy::util::Point> foggyFields;
            std::vector<spy::util::Point> destroyedWalls;
            std::vector<std::pair<spy::util::UUID, std::optional<double>>> spyResult; // if nullopt modify this value (no info about faction)
            std::vector<spy::util::Point> spyedSafes;
            spy::util::UUID movedMoledieTo;

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
