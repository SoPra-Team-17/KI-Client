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
        evalFunctions_caro() = delete;

        /**
         * evaluates item choice
         * @param offer character id or gadget type that got offered
         * @param config current match config
         * @param scenarioConfig current scenario config
         * @param characterConfig current character information
         * @return double indicating how good offer is (the higher the value the better)
         */
        static double itemChoice(const std::variant<const spy::util::UUID, const spy::gadget::GadgetEnum> &offer,
                                 const spy::MatchConfig &config,
                                 const spy::scenario::Scenario &scenarioConfig,
                                 const std::vector<spy::character::CharacterInformation> &characterConfig);

        /**
         * evaluates which character should get gadget in equipment choice
         * @param chosenCharacterIds ids of all characters that were chosen in the item choice phase
         * @param chosenGadgetType type of gadget that should be mapped to a character
         * @param config current match config
         * @param scenarioConfig current scenario config
         * @param characterConfig current character information
         * @return character that should get gadget
         */
        static spy::util::UUID equipmentChoice(const std::vector<spy::util::UUID> &chosenCharacterIds,
                                               spy::gadget::GadgetEnum chosenGadgetType, const spy::MatchConfig &config,
                                               const spy::scenario::Scenario &scenarioConfig,
                                               const std::vector<spy::character::CharacterInformation> &characterConfig);

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
        static double
        gameOperation(const spy::gameplay::State_AI &start, spy::gameplay::State_AI &s, const spy::util::UUID &characterId, const spy::MatchConfig &config,
                      const spy::scenario::Scenario &scenarioConfig,
                      const std::vector<spy::character::CharacterInformation> &characterConfig,
                      const libclient::LibClient &libClient);

    private:
        /**
         * helper for gameOption method
         * during the execution some Actions set nullopt into the state as they do not have enough information
         * these nullopts are replaced by values here, as we have got the necessary information here
         * @param s State_AI to processed
         */
        static void modifyNulloptsInState(spy::gameplay::State_AI &s);

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
        static double evalPosition(const spy::gameplay::State_AI &start, const spy::gameplay::State_AI &s,
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
        static double evalHp(const spy::gameplay::State_AI &s,
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
        static double evalUsedGadgets(const spy::gameplay::State_AI &s,
                                      const spy::MatchConfig &config,
                                      const libclient::LibClient &libClient);

        /**
         * helper for gameOperation method
         * evaluates the properties used by the character during its turn
         * @param s State_AI after character took its actions
         * @param libClient up to date libClient object for AIState information
         * @return double value to be added to return value of gameOperation
         */
        static double evalUsedProperties(const spy::gameplay::State_AI &s,
                                         const libclient::LibClient &libClient);

        /**
         * helper for gameOperation method
         * evaluates the spy results of the character during its turn
         * @param s State_AI after character took its actions
         * @param libClient up to date libClient object for AIState information
         * @return double value to be added to return value of gameOperation
         */
        static double evalSpy(const spy::gameplay::State_AI &s,
                              const libclient::LibClient &libClient);

        static constexpr double maxHealthPoints = 100.0;
        static constexpr double winningReason = 10;
        static constexpr double unsureLimit = 0.5;

        static double numMyChar;
        static double numEnemyChar;
        static double numUnknownChar;
        static double gadgetNullopt;
        static double enemyNullopt;
        static double npcNullopt;


        static void setStaticVars(const spy::scenario::Scenario &scenarioConfig, const spy::MatchConfig &config,
                                  const std::vector<spy::character::CharacterInformation> &characterConfig);

        static bool staticVarsSet;

        static double maxPlayingFieldDim;
        static double numBarTables;
        static double numRouletteTables;
        static double numWalls;
        static double numFree;
        static double numBarSeats;
        static double numSafes;
        static double numFireplaces;
        static double numFields;
        static std::vector<spy::util::Point> safePositions;
        static std::vector<spy::util::Point> roulettetablePositions;
        static std::vector<spy::util::Point> bartablePositions;

        static double midChipsPerRoulette;
        static double midChance;
        static double midHitChanceWithDamage;
        static double midRangeWithDamage;
        static double midDamage;

        static double numCharacter;
        static double numNimbleness;
        static double numSluggishness;
        static double numPonderousness;
        static double numSpryness;
        static double numAgility;
        static double numLuckyDevil;
        static double numJinx;
        static double numClammyClothes;
        static double numConstantClammyClothes;
        static double numRobustStomach;
        static double numToughness;
        static double numBabysitter;
        static double numHoneyTrap;
        static double numBangAndBurn;
        static double numFlapsAndSeals;
        static double numTradecraft;
        static double numObservation;

        static double maxChipsInCasino;
        static double maxIpInCasino;
        static double chipsToIpJudge;
        static double secretsToIpJudge;
};


#endif //KICLIENT_EVALFUNCTIONS_CARO_HPP
