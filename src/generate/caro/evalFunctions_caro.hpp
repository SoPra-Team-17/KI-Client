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

        static double itemChoice(const std::variant<const spy::util::UUID, const spy::gadget::GadgetEnum> &offer,
                                 const spy::MatchConfig &config,
                                 const spy::scenario::Scenario &scenarioConfig,
                                 const std::vector<spy::character::CharacterInformation> &characterConfig);

        static spy::util::UUID equipmentChoice(const std::vector<spy::util::UUID> &chosenCharacterIds,
                                               spy::gadget::GadgetEnum chosenGadgetType, const spy::MatchConfig &config,
                                               const spy::scenario::Scenario &scenarioConfig,
                                               const std::vector<spy::character::CharacterInformation> &characterConfig);

        static double
        gameOperation(const spy::gameplay::State_AI &start, spy::gameplay::State_AI &s, const spy::util::UUID &characterId, const spy::MatchConfig &config,
                      const spy::scenario::Scenario &scenarioConfig,
                      const std::vector<spy::character::CharacterInformation> &characterConfig,
                      const libclient::LibClient &libClient);

    private:
        static constexpr double maxHealthPoints = 100.0;

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
