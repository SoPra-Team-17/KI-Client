//
// Created by Carolin on 10.06.2020.
//

#ifndef KICLIENT_EVALFUNCTIONS_CARO_HPP
#define KICLIENT_EVALFUNCTIONS_CARO_HPP


#include <variant>
#include <datatypes/matchconfig/MatchConfig.hpp>
#include <datatypes/gadgets/GadgetEnum.hpp>
#include <util/UUID.hpp>
#include <datatypes/scenario/Scenario.hpp>
#include <datatypes/character/CharacterInformation.hpp>
#include <generate/utils/State_AI.hpp>

class evalFunctions_caro {
    public:
        evalFunctions_caro(const spy::MatchConfig &config, const spy::scenario::Scenario &scenarioConfig,
                           const std::vector<spy::character::CharacterInformation> &characterConfig);

        /**
         * evaluates item choice
         * @param offer character id or gadget type that got offered
         * @param config current match config
         * @param scenarioConfig current scenario config
         * @param characterConfig current character information
         * @return double indicating how good offer is (the higher the value the better)
         */
        [[nodiscard]] double itemChoice(const std::variant<const spy::util::UUID, const spy::gadget::GadgetEnum> &offer,
                                        const spy::MatchConfig &config,
                                        const std::vector<spy::character::CharacterInformation> &characterConfig) const;

        /**
         * evaluates which character should get gadget in equipment choice
         * @param chosenCharacterIds ids of all characters that were chosen in the item choice phase
         * @param chosenGadgetType type of gadget that should be mapped to a character
         * @param config current match config
         * @param scenarioConfig current scenario config
         * @param characterConfig current character information
         * @return character that should get gadget
         */
        [[nodiscard]] spy::util::UUID equipmentChoice(const std::vector<spy::util::UUID> &chosenCharacterIds,
                                                      spy::gadget::GadgetEnum chosenGadgetType,
                                                      const spy::MatchConfig &config,
                                                      const std::vector<spy::character::CharacterInformation> &characterConfig) const;

        /**
         * evaluates state that results after character took its actions
         * @param start State_AI before character took any actions
         * @param s State_AI after character took its actions
         * @param characterId id of the character that took actions
         * @param config current match config
         * @param scenarioConfig current scenario config
         * @param characterConfig current character information
         * @param libClient up to date libClient object for AIState information
         * @return double indicating how good resulting state is (the higher the value the better)
         */
        [[nodiscard]] double
        gameOperation(const spy::gameplay::State_AI &start, spy::gameplay::State_AI &s,
                      const spy::util::UUID &characterId, const spy::MatchConfig &config,
                      const std::vector<spy::character::CharacterInformation> &characterConfig,
                      const libclient::LibClient &libClient);

    private:
        /**
         * helper for gameOption method
         * during the execution some Actions set nullopt into the state as they do not have enough information
         * these nullopts are replaced by values here, as we have got the necessary information here
         * @param s State_AI to processed
         */
        void modifyNulloptsInState(spy::gameplay::State_AI &s);

        /**
         * helper for gameOperation method
         * evaluates the movement of the character
         * @param start State_AI before character took any actions
         * @param s State_AI after character took its actions
         * @param characterId id of the character that took actions
         * @param config current match config
         * @param libClient up to date libClient object for AIState information
         * @return double value to be added to return value of gameOperation
         */
        double evalPosition(const spy::gameplay::State_AI &start, const spy::gameplay::State_AI &s,
                            const spy::util::UUID &characterId,
                            const spy::MatchConfig &config,
                            const libclient::LibClient &libClient);

        /**
         * helper for gameOperation method
         * evaluates the health points that change during the turn of the character
         * @param s State_AI after character took its actions
         * @param characterId id of the character that took actions
         * @param libClient up to date libClient object for AIState information
         * @return double value to be added to return value of gameOperation
         */
        double evalHp(const spy::gameplay::State_AI &s,
                      const spy::util::UUID &characterId,
                      const libclient::LibClient &libClient);

        /**
         * helper for gameOperation method
         * evaluates the gadgets used by the character during its turn
         * @param s State_AI after character took its actions
         * @param config current match config
         * @param libClient up to date libClient object for AIState information
         * @return double value to be added to return value of gameOperation
         */
        double evalUsedGadgets(const spy::gameplay::State_AI &s,
                               const spy::MatchConfig &config,
                               const libclient::LibClient &libClient);

        /**
         * helper for gameOperation method
         * evaluates the properties used by the character during its turn
         * @param s State_AI after character took its actions
         * @param libClient up to date libClient object for AIState information
         * @return double value to be added to return value of gameOperation
         */
        double evalUsedProperties(const spy::gameplay::State_AI &s,
                                  const libclient::LibClient &libClient);

        /**
         * helper for gameOperation method
         * evaluates the spy results of the character during its turn
         * @param s State_AI after character took its actions
         * @param libClient up to date libClient object for AIState information
         * @return double value to be added to return value of gameOperation
         */
        double evalSpy(const spy::gameplay::State_AI &s,
                       const libclient::LibClient &libClient);

        static constexpr double maxHealthPoints = 100.0;
        static constexpr double winningReason = 10;
        static constexpr double unsureLimit = 0.5;

        double numMyChar;
        double numEnemyChar;
        double numUnknownChar;
        double gadgetNullopt;
        double enemyNullopt;
        double npcNullopt;

        double maxPlayingFieldDim;
        double numBarTables;
        double numRouletteTables;
        double numWalls;
        double numFree;
        double numBarSeats;
        double numSafes;
        double numFireplaces;
        double numFields;
        std::vector<spy::util::Point> safePositions;
        std::vector<spy::util::Point> roulettetablePositions;
        std::vector<spy::util::Point> bartablePositions;

        double midChipsPerRoulette;
        double midChance;
        double midHitChanceWithDamage;
        double midRangeWithDamage;
        double midDamage;

        double numCharacter;
        double numNimbleness;
        double numSluggishness;
        double numPonderousness;
        double numSpryness;
        double numAgility;
        double numLuckyDevil;
        double numJinx;
        double numClammyClothes;
        double numConstantClammyClothes;
        double numRobustStomach;
        double numToughness;
        double numBabysitter;
        double numHoneyTrap;
        double numBangAndBurn;
        double numFlapsAndSeals;
        double numTradecraft;
        double numObservation;

        double maxChipsInCasino;
        double maxIpInCasino;
        double chipsToIpJudge;
        double secretsToIpJudge;
};


#endif //KICLIENT_EVALFUNCTIONS_CARO_HPP
