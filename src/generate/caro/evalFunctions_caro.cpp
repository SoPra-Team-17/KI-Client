//
// Created by Carolin on 10.06.2020.
//

#include "evalFunctions_caro.hpp"
#include <util/GameLogicUtils.hpp>
#include <datatypes/gadgets/Cocktail.hpp>

bool evalFunctions_caro::staticVarsSet = false;

double evalFunctions_caro::maxPlayingFieldDim = 0;
double evalFunctions_caro::numBarTables = 0;
double evalFunctions_caro::numRouletteTables = 0;
double evalFunctions_caro::numWalls = 0;
double evalFunctions_caro::numFree = 0;
double evalFunctions_caro::numBarSeats = 0;
double evalFunctions_caro::numSafes = 0;
double evalFunctions_caro::numFireplaces = 0;
double evalFunctions_caro::numFields = 0;
std::vector<spy::util::Point> evalFunctions_caro::safePositions = {};
std::vector<spy::util::Point> evalFunctions_caro::roulettetablePositions = {};
std::vector<spy::util::Point> evalFunctions_caro::bartablePositions = {};

double evalFunctions_caro::midChipsPerRoulette = 0;
double evalFunctions_caro::midChance = 0;
double evalFunctions_caro::midHitChanceWithDamage = 0;
double evalFunctions_caro::midRangeWithDamage = 0;
double evalFunctions_caro::midDamage = 0;

double evalFunctions_caro::numCharacter = 0;
double evalFunctions_caro::numNimbleness = 0;
double evalFunctions_caro::numSluggishness = 0;
double evalFunctions_caro::numPonderousness = 0;
double evalFunctions_caro::numSpryness = 0;
double evalFunctions_caro::numAgility = 0;
double evalFunctions_caro::numLuckyDevil = 0;
double evalFunctions_caro::numJinx = 0;
double evalFunctions_caro::numClammyClothes = 0;
double evalFunctions_caro::numConstantClammyClothes = 0;
double evalFunctions_caro::numRobustStomach = 0;
double evalFunctions_caro::numToughness = 0;
double evalFunctions_caro::numBabysitter = 0;
double evalFunctions_caro::numHoneyTrap = 0;
double evalFunctions_caro::numBangAndBurn = 0;
double evalFunctions_caro::numFlapsAndSeals = 0;
double evalFunctions_caro::numTradecraft = 0;
double evalFunctions_caro::numObservation = 0;

double evalFunctions_caro::maxChipsInCasino;
double evalFunctions_caro::chipsToIpJudge;

double evalFunctions_caro::itemChoice(const std::variant<const spy::util::UUID, const spy::gadget::GadgetEnum> &offer,
                                      const spy::MatchConfig &config, const spy::scenario::Scenario &scenarioConfig,
                                      const std::vector<spy::character::CharacterInformation> &characterConfig) {
    if (!staticVarsSet) {
        setStaticVars(scenarioConfig, config, characterConfig);
    }

    double ret = 0;

    if (std::holds_alternative<const spy::util::UUID>(offer)) {
        auto characterId = std::get<const spy::util::UUID>(offer);

        auto character = std::find_if(characterConfig.begin(), characterConfig.end(),
                                      [&characterId](const spy::character::CharacterInformation &info) {
                                          return info.getCharacterId() == characterId;
                                      });
        auto props = character->getFeatures();

        for (const auto &p: props) {
            switch (p) {
                // AP and BP
                case spy::character::PropertyEnum::NIMBLENESS:
                    ret += 1 - numNimbleness / numCharacter;
                    break;
                case spy::character::PropertyEnum::SLUGGISHNESS:
                    ret -= 1 - numSluggishness / numCharacter;
                    break;
                case spy::character::PropertyEnum::SPRYNESS:
                    ret += 1 - numSpryness / numCharacter;
                    break;
                case spy::character::PropertyEnum::PONDEROUSNESS:
                    ret -= 1 - numPonderousness / numCharacter;
                    break;
                case spy::character::PropertyEnum::AGILITY:
                    ret += 1 - numAgility / numCharacter;
                    break;
                    // roulette
                case spy::character::PropertyEnum::LUCKY_DEVIL:
                    ret += numRouletteTables / numFields * (1 - numLuckyDevil / numCharacter) * chipsToIpJudge *
                           midChipsPerRoulette / maxChipsInCasino;
                    break;
                case spy::character::PropertyEnum::JINX:
                    ret -= numRouletteTables / numFields * (1 - numJinx / numCharacter) * chipsToIpJudge *
                           midChipsPerRoulette / maxChipsInCasino;
                    break;
                case spy::character::PropertyEnum::BANG_AND_BURN:
                    if (props.find(spy::character::PropertyEnum::LUCKY_DEVIL) != props.end()) {
                        ret -= numRouletteTables / numFields * (1 - numBangAndBurn / numCharacter);
                    }
                    if (props.find(spy::character::PropertyEnum::JINX) != props.end()) {
                        ret += numRouletteTables / numFields * (1 - numBangAndBurn / numCharacter);
                    }
                    break;
                    // chance
                case spy::character::PropertyEnum::CLAMMY_CLOTHES:
                    ret -= midChance * (1 - numClammyClothes / numCharacter);
                    break;
                case spy::character::PropertyEnum::CONSTANT_CLAMMY_CLOTHES:
                    ret -= midChance * (1 - numConstantClammyClothes / numCharacter);
                    break;
                case spy::character::PropertyEnum::TRADECRAFT:
                    ret += midChance * (1 - numTradecraft / numCharacter);
                    break;
                    // HP
                case spy::character::PropertyEnum::ROBUST_STOMACH:
                    ret += config.getCocktailHealthPoints() * numBarTables / numFields *
                           (1 - numRobustStomach / numCharacter);
                    break;
                case spy::character::PropertyEnum::TOUGHNESS:
                    ret += midDamage * (1 - numToughness / numCharacter);
                    break;
                    // protection
                case spy::character::PropertyEnum::BABYSITTER:
                    ret += midHitChanceWithDamage * config.getBabysitterSuccessChance() *
                           (1 - numBabysitter / numCharacter);
                    break;
                case spy::character::PropertyEnum::HONEY_TRAP:
                    ret += config.getHoneyTrapSuccessChance() * (1 - numHoneyTrap / numCharacter);
                    break;
                    // safes
                case spy::character::PropertyEnum::FLAPS_AND_SEALS:
                    ret += numSafes / numFields * (1 - numFlapsAndSeals / numCharacter);
                    break;
                    // finding out faction
                case spy::character::PropertyEnum::OBSERVATION:
                    ret += config.getObservationSuccessChance() * (1 - numObservation / numCharacter);
                    break;
                default:
                    break;
            }
        }
    }

    if (std::holds_alternative<const spy::gadget::GadgetEnum>(offer)) {
        auto gadget = std::get<const spy::gadget::GadgetEnum>(offer);

        switch (gadget) {
            case spy::gadget::GadgetEnum::HAIRDRYER:
                ret += midChance * (1 - config.getCocktailDodgeChance()) * numBarTables / numFields;
                break;
            case spy::gadget::GadgetEnum::MOLEDIE:
                ret -= std::min(1.0, static_cast<double>(config.getMoledieRange() / maxPlayingFieldDim) * 1) /
                       static_cast<double>(config.getRoundLimit());
                break;
            case spy::gadget::GadgetEnum::TECHNICOLOUR_PRISM:
                ret += numRouletteTables / numFields * chipsToIpJudge * midChipsPerRoulette / maxChipsInCasino * 1 /
                       numRouletteTables;
                break;
            case spy::gadget::GadgetEnum::BOWLER_BLADE:
                ret += config.getBowlerBladeHitChance() *
                       std::min(1.0, static_cast<double>(config.getBowlerBladeDamage() / 100)) *
                       std::min(1.0, static_cast<double>(config.getBowlerBladeRange() / maxPlayingFieldDim) * 1) /
                       static_cast<double>(config.getRoundLimit());
                break;
            case spy::gadget::GadgetEnum::MAGNETIC_WATCH:
                ret += config.getBowlerBladeHitChance() *
                       std::min(1.0, static_cast<double>(config.getBowlerBladeDamage() / 100)) *
                       std::min(1.0, static_cast<double>(config.getBowlerBladeRange() / maxPlayingFieldDim));
                break;
            case spy::gadget::GadgetEnum::POISON_PILLS:
                ret += std::min(1.0,
                                static_cast<double>(config.getCocktailHealthPoints() / 100) * numBarTables / numFields *
                                5) /
                       static_cast<double>(config.getRoundLimit());
                break;
            case spy::gadget::GadgetEnum::LASER_COMPACT:
                ret += config.getLaserCompactHitChance() * numBarTables / numFields;
                break;
            case spy::gadget::GadgetEnum::ROCKET_PEN:
                ret += std::min(1.0, static_cast<double>(config.getRocketPenDamage() / 100) * 1 /
                                     static_cast<double>(config.getRoundLimit()));
                break;
            case spy::gadget::GadgetEnum::GAS_GLOSS:
                ret += std::min(1.0, static_cast<double>(config.getGasGlossDamage() / 100) * 1 /
                                     static_cast<double>(config.getRoundLimit()));
                break;
            case spy::gadget::GadgetEnum::MOTHBALL_POUCH:
                ret += std::min(1.0, static_cast<double>(config.getMothballPouchRange() / maxPlayingFieldDim)) *
                       std::min(1.0, static_cast<double>(config.getMothballPouchDamage() / 100)) *
                       numFireplaces / numFields * 5 / static_cast<double>(config.getRoundLimit());
                break;
            case spy::gadget::GadgetEnum::FOG_TIN:
                ret += std::min(1.0, static_cast<double>(config.getFogTinRange() / maxPlayingFieldDim) * 3) /
                       static_cast<double>(config.getRoundLimit());
                break;
            case spy::gadget::GadgetEnum::GRAPPLE:
                ret += config.getGrappleHitChance() *
                       std::min(1.0, static_cast<double>(config.getGrappleRange() / maxPlayingFieldDim));
                break;
            case spy::gadget::GadgetEnum::WIRETAP_WITH_EARPLUGS:
                ret += (1 - config.getWiretapWithEarplugsFailChance()) * 1 /
                       static_cast<double>(config.getRoundLimit());
                break;
            case spy::gadget::GadgetEnum::JETPACK:
                ret += 1 / static_cast<double>(config.getRoundLimit());
                break;
            case spy::gadget::GadgetEnum::CHICKEN_FEED:
                ret += 2 / (numCharacter - 2) * 1 / static_cast<double>(config.getRoundLimit());
                break;
            case spy::gadget::GadgetEnum::NUGGET:
                ret += (1 - 4 / (numCharacter - 2)) * 1 / static_cast<double>(config.getRoundLimit());
                break;
            case spy::gadget::GadgetEnum::MIRROR_OF_WILDERNESS:
                ret += config.getMirrorSwapChance() * 1 / static_cast<double>(config.getRoundLimit());
                break;
            case spy::gadget::GadgetEnum::POCKET_LITTER:
                ret += config.getObservationSuccessChance() * numObservation / numCharacter;
                break;
            case spy::gadget::GadgetEnum::ANTI_PLAGUE_MASK:
                ret += 10 / 100 * (1 - numBarSeats / numFields);
                break;
            default:
                break;
        }

    }

    return ret;
}

spy::util::UUID evalFunctions_caro::equipmentChoice(const std::vector<spy::util::UUID> &chosenCharacterIds,
                                                    spy::gadget::GadgetEnum chosenGadgetType,
                                                    const spy::MatchConfig &config,
                                                    const spy::scenario::Scenario &scenarioConfig,
                                                    const std::vector<spy::character::CharacterInformation> &characterConfig) {

    std::set<spy::character::PropertyEnum> posProps = {spy::character::PropertyEnum::SPRYNESS};
    std::set<spy::character::PropertyEnum> negProps;

    switch (chosenGadgetType) {
        case spy::gadget::GadgetEnum::HAIRDRYER:
            negProps.insert(spy::character::PropertyEnum::CONSTANT_CLAMMY_CLOTHES);
            posProps.insert(spy::character::PropertyEnum::CLAMMY_CLOTHES);
            break;
        case spy::gadget::GadgetEnum::MOLEDIE:
            negProps.insert(spy::character::PropertyEnum::TRADECRAFT);
            negProps.insert(spy::character::PropertyEnum::FLAPS_AND_SEALS);
            negProps.insert(spy::character::PropertyEnum::OBSERVATION);
            break;
        case spy::gadget::GadgetEnum::TECHNICOLOUR_PRISM:
            negProps.insert(spy::character::PropertyEnum::LUCKY_DEVIL);
            posProps.insert(spy::character::PropertyEnum::JINX);
            break;
        case spy::gadget::GadgetEnum::BOWLER_BLADE:
            negProps.insert(spy::character::PropertyEnum::CLAMMY_CLOTHES);
            negProps.insert(spy::character::PropertyEnum::CONSTANT_CLAMMY_CLOTHES);
            posProps.insert(spy::character::PropertyEnum::TRADECRAFT);
            break;
        case spy::gadget::GadgetEnum::MAGNETIC_WATCH:
            negProps.insert(spy::character::PropertyEnum::TOUGHNESS);
            negProps.insert(spy::character::PropertyEnum::HONEY_TRAP);
            break;
        case spy::gadget::GadgetEnum::POISON_PILLS:
            break;
        case spy::gadget::GadgetEnum::LASER_COMPACT:
            negProps.insert(spy::character::PropertyEnum::CLAMMY_CLOTHES);
            negProps.insert(spy::character::PropertyEnum::CONSTANT_CLAMMY_CLOTHES);
            posProps.insert(spy::character::PropertyEnum::TRADECRAFT);
            break;
        case spy::gadget::GadgetEnum::ROCKET_PEN:
            negProps.insert(spy::character::PropertyEnum::NIMBLENESS);
            posProps.insert(spy::character::PropertyEnum::SLUGGISHNESS);
            negProps.insert(spy::character::PropertyEnum::AGILITY);
            posProps.insert(spy::character::PropertyEnum::PONDEROUSNESS);
            break;
        case spy::gadget::GadgetEnum::GAS_GLOSS:
            break;
        case spy::gadget::GadgetEnum::MOTHBALL_POUCH:
            break;
        case spy::gadget::GadgetEnum::FOG_TIN:
            break;
        case spy::gadget::GadgetEnum::GRAPPLE:
            negProps.insert(spy::character::PropertyEnum::CLAMMY_CLOTHES);
            negProps.insert(spy::character::PropertyEnum::CONSTANT_CLAMMY_CLOTHES);
            posProps.insert(spy::character::PropertyEnum::TRADECRAFT);
            break;
        case spy::gadget::GadgetEnum::WIRETAP_WITH_EARPLUGS:
            break;
        case spy::gadget::GadgetEnum::JETPACK:
            negProps.insert(spy::character::PropertyEnum::NIMBLENESS);
            posProps.insert(spy::character::PropertyEnum::SLUGGISHNESS);
            negProps.insert(spy::character::PropertyEnum::AGILITY);
            posProps.insert(spy::character::PropertyEnum::PONDEROUSNESS);
            break;
        case spy::gadget::GadgetEnum::CHICKEN_FEED:
            posProps.insert(spy::character::PropertyEnum::OBSERVATION);
            break;
        case spy::gadget::GadgetEnum::NUGGET:
            posProps.insert(spy::character::PropertyEnum::OBSERVATION);
            break;
        case spy::gadget::GadgetEnum::MIRROR_OF_WILDERNESS:
            negProps.insert(spy::character::PropertyEnum::CLAMMY_CLOTHES);
            negProps.insert(spy::character::PropertyEnum::CONSTANT_CLAMMY_CLOTHES);
            posProps.insert(spy::character::PropertyEnum::TRADECRAFT);
            break;
        case spy::gadget::GadgetEnum::POCKET_LITTER: {
            // character with best val in item function
            spy::util::UUID character;
            double characterVal = -std::numeric_limits<double>::infinity();
            for (const auto &charId: chosenCharacterIds) {
                double val = itemChoice(charId, config, scenarioConfig, characterConfig);
                if (val > characterVal) {
                    characterVal = val;
                    character = charId;
                }
            }
            return character;
        }
        case spy::gadget::GadgetEnum::ANTI_PLAGUE_MASK:
            negProps.insert(spy::character::PropertyEnum::ROBUST_STOMACH);
            negProps.insert(spy::character::PropertyEnum::TOUGHNESS);
            negProps.insert(spy::character::PropertyEnum::HONEY_TRAP);
            break;
        default:
            break;
    }


    // decide on character according to properties
    spy::util::UUID retChar;
    double retVal = -std::numeric_limits<double>::infinity();
    for (const auto &characterId: chosenCharacterIds) {
        auto character = std::find_if(characterConfig.begin(), characterConfig.end(),
                                      [&characterId](const spy::character::CharacterInformation &info) {
                                          return info.getCharacterId() == characterId;
                                      });
        auto props = character->getFeatures();
        auto val = 0;

        for (const auto &pp: posProps) {
            if (props.find(pp) != props.end()) {
                val += 1;
            }
        }
        for (const auto &np: negProps) {
            if (props.find(np) != props.end()) {
                val -= 1;
            }
        }
        if (props.find(spy::character::PropertyEnum::PONDEROUSNESS) != props.end()) {
            val -= 0.5;
        }
        if (props.find(spy::character::PropertyEnum::AGILITY) != props.end()) {
            val += 0.5;
        }

        if (val > retVal) {
            retVal = val;
            retChar = characterId;
        }

    }

    return retChar;
}

double evalFunctions_caro::gameOperation(const spy::gameplay::State_AI &start, spy::gameplay::State_AI &s,
                                         const spy::util::UUID &characterId,
                                         const spy::MatchConfig &config,
                                         const spy::scenario::Scenario &scenarioConfig,
                                         const std::vector<spy::character::CharacterInformation> &characterConfig,
                                         const libclient::LibClient &libClient) {
    double retVal = 0;

    // modify nullopts in state
    auto numMyChar = static_cast<double>(libClient.getMyFactionList().size());
    auto numEnemyChar = static_cast<double>(libClient.getEnemyFactionList().size());
    auto numUnknownChar = static_cast<double>(libClient.getUnknownFactionList().size());
    double gadgetNullopt = 1 / (numCharacter - numMyChar);
    double enemyNullopt = std::min(1.0, (std::max(0.0, 4 - numEnemyChar) + std::max(0.0, 3 - numEnemyChar) +
                                         std::max(0.0, 2 - numEnemyChar)) / (3 * numUnknownChar));
    double npcNullopt = 1 - enemyNullopt;
    for (auto &val: s.observationResult) {
        if (!val.has_value()) {
            val = gadgetNullopt;
        }
    }
    if (!s.nuggetResult.has_value()) {
        s.nuggetResult = npcNullopt;
    }
    for (auto &val: s.spyResult) {
        if (!val.second.has_value()) {
            val.second = npcNullopt;
        }
    }
    if (!s.chickenfeedResult.first.has_value()) {
        s.chickenfeedResult.first = enemyNullopt;
    }
    s.stateChance *= std::pow(gadgetNullopt, s.unknownGadgetsModifyingSuccess);

    auto myCharacter = s.getCharacters().findByUUID(characterId);
    const auto &getCombisFromNpcs = libClient.getCombinationsFromNpcs();
    const auto &openedSafes = libClient.getOpenedSafes();
    const auto &triedSafes = libClient.getTriedSafes();
    const auto &safeCombis = libClient.getSafeCombinations();

    // collected gadgets that were not used
    auto used = s.usedGadgets;
    for (auto &gad: s.collectedGadgets) {
        auto g = std::find(used.begin(), used.end(), gad);
        if (g != used.end()) {
            used.erase(g);
            retVal += itemChoice(gad, config, scenarioConfig, characterConfig);
            if (gad == spy::gadget::GadgetEnum::DIAMOND_COLLAR) {
                retVal += config.getCatIp();
            } else if (gad == spy::gadget::GadgetEnum::COCKTAIL) {
                retVal += config.getCocktailHealthPoints();
            }
        }
    }

    // health points
    if (myCharacter->getHealthPoints() < 0) {
        return -std::numeric_limits<double>::infinity(); // do not kill yourself
    }
    double myHp = 0;
    double enemyHp = 0;
    for (const auto &pair: s.hpDiff) {
        if (libClient.hasCharacterFaction(pair.first, spy::character::FactionEnum::PLAYER1).value() == 1) {
            myHp += pair.second;
        } else {
            auto isEnemy = libClient.hasCharacterFaction(pair.first, spy::character::FactionEnum::PLAYER2);
            if (isEnemy.has_value()) {
                enemyHp += pair.second * isEnemy.value();
            } else {
                enemyHp += pair.second * enemyNullopt;
            }
        }
    }
    if (enemyHp >= myHp) {
        return -std::numeric_limits<double>::infinity(); // do not damage your faction more than enemy faction
    }
    retVal += myHp - enemyHp;

    // movement
    spy::util::Point startPoint = start.getCharacters().findByUUID(characterId)->getCoordinates().value();
    spy::util::Point endPoint = myCharacter->getCoordinates().value();
    if (endPoint != startPoint) {
        auto endField = s.getMap().getField(endPoint);

        if (s.getJanitorCoordinates().has_value()) {
            // janitor is on field
            auto startJanitorTargets = spy::util::GameLogicUtils::getCharacterNearFields(start,
                                                                                         s.getJanitorCoordinates().value());
            auto endJanitorTargets = spy::util::GameLogicUtils::getCharacterNearFields(s,
                                                                                       s.getJanitorCoordinates().value());
            bool inStart = std::find(startJanitorTargets.begin(), startJanitorTargets.end(), startPoint) !=
                           startJanitorTargets.end();
            bool inEnd =
                    std::find(endJanitorTargets.begin(), endJanitorTargets.end(), endPoint) != endJanitorTargets.end();

            if (inStart && !inEnd) {
                retVal += 10;
            } else if (!inStart && inEnd) {
                return -std::numeric_limits<double>::infinity(); // do not walk to janitor
            }
        }

        if (s.getCatCoordinates().has_value() &&
            myCharacter->hasGadget(spy::gadget::GadgetEnum::DIAMOND_COLLAR)) {
            auto startDistToCat = spy::gameplay::Movement::getMoveDistance(s.getCatCoordinates().value(), startPoint);
            auto endDistToCat = spy::gameplay::Movement::getMoveDistance(s.getCatCoordinates().value(), endPoint);
            retVal += startDistToCat - endDistToCat;
        }

        if (myCharacter->hasProperty(spy::character::PropertyEnum::CLAMMY_CLOTHES) &&
            !myCharacter->hasProperty(spy::character::PropertyEnum::CONSTANT_CLAMMY_CLOTHES)) {

            bool endFireplace = endField.getFieldState() == spy::scenario::FieldStateEnum::FIREPLACE;
            retVal += endFireplace ? midChance : -midChance;
        }

        bool endBarseat = endField.getFieldState() == spy::scenario::FieldStateEnum::BAR_SEAT;
        double value = (1 - static_cast<double>(myCharacter->getHealthPoints()) / 100);
        retVal += endBarseat ? value : -value;

        for (const auto &p: safePositions) {
            auto safeIndex = libClient.safePosToIndex(s, p);
            if (openedSafes.find(safeIndex) != openedSafes.end() && triedSafes.find(safeIndex) != triedSafes.end() &&
                triedSafes.at(safeIndex) == static_cast<int>(safeCombis.size())) {
                continue; // getting close to already opened tried safes is not useful
            }
            auto startDist = spy::gameplay::Movement::getMoveDistance(p, startPoint);
            auto endDist = spy::gameplay::Movement::getMoveDistance(p, endPoint);
            double val = 1 / (numSafes - static_cast<double>(openedSafes.size()));
            retVal += endDist < startDist ? val : -val;
        }

        auto npcList = libClient.getNpcFactionList();
        for (const auto &npcId: npcList) {
            auto npc = s.getCharacters().findByUUID(npcId)->getCoordinates();
            if (npc.has_value()) {
                auto startDist = spy::gameplay::Movement::getMoveDistance(npc.value(), startPoint);
                auto endDist = spy::gameplay::Movement::getMoveDistance(npc.value(), endPoint);
                double val = config.getSpySuccessChance();
                retVal += endDist < startDist ? val : -val;
            }
        }

        for (const auto &p: roulettetablePositions) {
            auto roulette = s.getMap().getField(p);
            if (!roulette.isDestroyed().value()) {
                auto startDist = spy::gameplay::Movement::getMoveDistance(p, startPoint);
                auto endDist = spy::gameplay::Movement::getMoveDistance(p, endPoint);
                double val = 18.0/37.0;
                if (myCharacter->hasProperty(spy::character::PropertyEnum::LUCKY_DEVIL)) {
                    val = 32.0/37.0;
                } else if (myCharacter->hasProperty(spy::character::PropertyEnum::JINX)) {
                    val = 13.0/37.0;
                }
                val = roulette.isInverted().value() ? (1-val) : val;
                retVal += endDist < startDist ? val : -val;
            }
        }

        for (const auto &p: bartablePositions) {
            auto bar = s.getMap().getField(p);
            if (bar.getGadget().has_value()) {
                auto cocktail = std::dynamic_pointer_cast<spy::gadget::Cocktail>(bar.getGadget().value())->isPoisoned();
                double val = config.getCocktailHealthPoints();
                retVal += cocktail ? val : -val;
            }
        }
    }

    // gadget
    for (auto &usedG: s.usedGadgets) {
        switch (usedG) {
            case spy::gadget::GadgetEnum::HAIRDRYER:
                for (const auto &id: s.removedClammyClothes) {
                    if (libClient.hasCharacterFaction(id, spy::character::FactionEnum::PLAYER1).value() == 1) {
                        retVal += midChance;
                    } else {
                        return -std::numeric_limits<double>::infinity(); // do not dry persons that are not your faction
                    }
                }
                break;
            case spy::gadget::GadgetEnum::MOLEDIE:
                if (libClient.hasCharacterFaction(s.movedMoledieTo, spy::character::FactionEnum::PLAYER1).value() ==
                    1) {
                    return -std::numeric_limits<double>::infinity(); // do not put gadget to person of own faction
                } else {
                    retVal += midChance;
                }
                break;
            case spy::gadget::GadgetEnum::TECHNICOLOUR_PRISM: {
                int counter = 0;
                auto myFaction = libClient.getMyFactionList();
                for (const auto &id: myFaction) {
                    auto myFactionChar = s.getCharacters().findByUUID(id);
                    if (myFactionChar->getCoordinates().has_value()) {
                        auto val = (maxPlayingFieldDim -
                                    spy::gameplay::Movement::getMoveDistance(myFactionChar->getCoordinates().value(),
                                                                             s.invertedRoulette));
                        if (myFactionChar->hasProperty(spy::character::PropertyEnum::LUCKY_DEVIL)) {
                            counter -= val;
                        }
                        if (myFactionChar->hasProperty(spy::character::PropertyEnum::JINX)) {
                            counter += val;
                        }
                    }
                }
                if (counter != 0) {
                    double val = s.getMap().getField(s.invertedRoulette).getChipAmount().value() / maxChipsInCasino;
                    retVal += counter > 0 ? val : -val;
                }
                break;
            }
            case spy::gadget::GadgetEnum::BOWLER_BLADE:
                // already in health points eval
                break;
            case spy::gadget::GadgetEnum::POISON_PILLS:
                for (const auto &val: s.poisonedCocktails) {
                    if (std::holds_alternative<spy::util::UUID>(val)) {
                        if (libClient.hasCharacterFaction(std::get<spy::util::UUID>(val),
                                                          spy::character::FactionEnum::PLAYER1).value() == 1) {
                            return -std::numeric_limits<double>::infinity(); // do not poison cocktail hold by person of your faction
                        } else {
                            retVal += config.getCocktailHealthPoints();
                        }
                    }
                }
                break;
            case spy::gadget::GadgetEnum::LASER_COMPACT:
                for (const auto &pair: s.removedCocktails) {
                    if (std::holds_alternative<spy::util::UUID>(pair.first)) {
                        if (libClient.hasCharacterFaction(std::get<spy::util::UUID>(pair.first),
                                                          spy::character::FactionEnum::PLAYER1).value() == 1) {
                            if (!pair.second) {
                                return -std::numeric_limits<double>::infinity(); // do not remove healthy cocktail hold by person of your faction
                            } else {
                                retVal += config.getCocktailHealthPoints(); // do remove poisoned cocktail hold by person of your faction
                            }
                        } else if (!pair.second) {
                            return -std::numeric_limits<double>::infinity(); // do not remove poisoned cocktail hold by person of other faction
                        } else if (pair.second) {
                            retVal += config.getCocktailHealthPoints(); // do remove healthy cocktail hold by person of other faction
                        }
                    }
                }
                break;
            case spy::gadget::GadgetEnum::ROCKET_PEN:
                // already in movement eval
                break;
            case spy::gadget::GadgetEnum::GAS_GLOSS:
                // already in health points eval
                break;
            case spy::gadget::GadgetEnum::MOTHBALL_POUCH:
                // already in health points eval
                break;
            case spy::gadget::GadgetEnum::FOG_TIN: {
                double myCounter = 0;
                double enemyCounter = 0;
                for (const auto &p: s.foggyFields) {
                    auto character = spy::util::GameLogicUtils::findInCharacterSetByCoordinates(s.getCharacters(), p);
                    if (character != s.getCharacters().end()) {
                        // character on field
                        if (libClient.hasCharacterFaction(character->getCharacterId(),
                                                          spy::character::FactionEnum::PLAYER1).value() == 1) {
                            myCounter += 1;
                        } else {
                            auto isEnemy = libClient.hasCharacterFaction(character->getCharacterId(),
                                                                         spy::character::FactionEnum::PLAYER2);
                            if (isEnemy.has_value()) {
                                enemyCounter += isEnemy.value();
                            } else {
                                enemyCounter += enemyNullopt;
                            }
                        }
                    }
                }

                if (myCounter >= enemyCounter) {
                    return -std::numeric_limits<double>::infinity(); // do not set your faction more in fog than enemy faction
                }
                retVal += enemyNullopt - myCounter;
                break;
            }
            case spy::gadget::GadgetEnum::GRAPPLE:
                // already in collectedGadgets eval
                break;
            case spy::gadget::GadgetEnum::WIRETAP_WITH_EARPLUGS:
                retVal += 1 - config.getWiretapWithEarplugsFailChance();
                break;
            case spy::gadget::GadgetEnum::DIAMOND_COLLAR:
                retVal += config.getCatIp();
                break;
            case spy::gadget::GadgetEnum::JETPACK:
                // already in movement eval
                break;
            case spy::gadget::GadgetEnum::CHICKEN_FEED:
                if (s.chickenfeedResult.first.has_value()) {
                    if (s.chickenfeedResult.first.value() <= 0.5) {
                        return -std::numeric_limits<double>::infinity(); // to unsure, do not waste chicken feed
                    } else {
                        retVal += s.chickenfeedResult.first.value() - 0.5;
                    }
                }
                if (s.chickenfeedResult.second.has_value()) {
                    if (libClient.hasCharacterFaction(s.chickenfeedResult.second.value(),
                                                      spy::character::FactionEnum::PLAYER1) == 1) {
                        retVal += s.chickenfeedResult.first.value();
                    } else {
                        return -std::numeric_limits<double>::infinity(); // do not give ip to other factions
                    }
                }
                break;
            case spy::gadget::GadgetEnum::NUGGET:
                if (s.nuggetResult.has_value()) {
                    if (s.nuggetResult.value() <= 0.5) {
                        return -std::numeric_limits<double>::infinity(); // to unsure to use nugget
                    } else {
                        retVal += s.nuggetResult.value() - 0.5;
                    }
                }
                break;
            case spy::gadget::GadgetEnum::MIRROR_OF_WILDERNESS:
                if (s.mowResult.second.has_value()) {
                    if (libClient.hasCharacterFaction(s.mowResult.second.value(),
                                                      spy::character::FactionEnum::PLAYER1) == 1) {
                        retVal += s.mowResult.first;
                    } else {
                        return -std::numeric_limits<double>::infinity(); // do not give ip to other factions
                    }
                }
                retVal += s.mowResult.first;
                break;
            case spy::gadget::GadgetEnum::COCKTAIL:
                for (const auto &id: s.addedClammyClothes) {
                    if (libClient.hasCharacterFaction(id, spy::character::FactionEnum::PLAYER1).value() == 1) {
                        return -std::numeric_limits<double>::infinity(); // do not wet persons that are in your faction
                    } else {
                        retVal += midChance * 10;
                    }
                }
                break;
            default:
                break;
        }
    }

    // gamble
    retVal += s.chipDiff * config.getChipsToIpFactor();

    // property observation
    for (const auto &val: s.observationResult) {
        if (val.value() <= 0.5) {
            return -std::numeric_limits<double>::infinity(); // to unsure to use observation
        } else {
            retVal += val.value() - 0.5;
        }
    }
    // property bang and burn
    auto myFaction = libClient.getMyFactionList();
    for (const auto &p: s.destroyedRoulettes) {
        int counter = 0;
        for (const auto &id: myFaction) {
            auto myFactionChar = s.getCharacters().findByUUID(id);
            if (myFactionChar->getCoordinates().has_value()) {
                auto val = (maxPlayingFieldDim -
                            spy::gameplay::Movement::getMoveDistance(myFactionChar->getCoordinates().value(), p));
                if (myFactionChar->hasProperty(spy::character::PropertyEnum::LUCKY_DEVIL)) {
                    counter -= val;
                }
                if (myFactionChar->hasProperty(spy::character::PropertyEnum::JINX)) {
                    counter += val;
                }
            }
        }
        if (counter != 0) {
            double val = s.getMap().getField(p).getChipAmount().value() / maxChipsInCasino;
            retVal += counter > 0 ? val : -val;
        }
    }

    // spy
    for (const auto &pair: s.spyResult) {
        if (pair.second.value() <= 0.5) {
            return -std::numeric_limits<double>::infinity(); // to unsure to spy out
        } else {
            if (getCombisFromNpcs.find(pair.first) != getCombisFromNpcs.end()) {
                retVal += (pair.second.value() - 0.5) * 0.5;
            } else {
                retVal += (pair.second.value() - 0.5) * config.getSecretToIpFactor();
            }
        }
    }
    for (const auto &p: s.spyedSafes) {
        auto safeIndex = libClient.safePosToIndex(s, p);
        if (openedSafes.find(safeIndex) != openedSafes.end()) {
            return -std::numeric_limits<double>::infinity(); // do not open already opened safes
        }
        if (triedSafes.find(safeIndex) != triedSafes.end()) {
            if (triedSafes.at(safeIndex) == static_cast<int>(safeCombis.size())) {
                return -std::numeric_limits<double>::infinity(); // do not reopen already tried safes without new combis
            }
        }
        retVal += 1 / (numSafes - static_cast<double>(openedSafes.size()));
    }

    return retVal * s.stateChance;
}

void evalFunctions_caro::setStaticVars(const spy::scenario::Scenario &scenarioConfig, const spy::MatchConfig &config,
                                       const std::vector<spy::character::CharacterInformation> &characterConfig) {
    // scenario
    maxPlayingFieldDim = scenarioConfig.getNumberOfRows();
    for (auto y = 0U; y < scenarioConfig.getNumberOfRows(); y++) {
        for (auto x = 0U; x < scenarioConfig.getRowLength(y); x++) {

            if (x > maxPlayingFieldDim) {
                maxPlayingFieldDim = x;
            }

            numFields++;

            switch (scenarioConfig.getField(x, y)) {
                case spy::scenario::FieldStateEnum::BAR_TABLE:
                    bartablePositions.push_back({static_cast<int>(x), static_cast<int>(y)});
                    numBarTables++;
                    break;
                case spy::scenario::FieldStateEnum::ROULETTE_TABLE:
                    roulettetablePositions.push_back({static_cast<int>(x), static_cast<int>(y)});
                    numRouletteTables++;
                    break;
                case spy::scenario::FieldStateEnum::WALL:
                    numWalls++;
                    break;
                case spy::scenario::FieldStateEnum::FREE:
                    numFree++;
                    break;
                case spy::scenario::FieldStateEnum::BAR_SEAT:
                    numBarSeats++;
                    break;
                case spy::scenario::FieldStateEnum::SAFE:
                    safePositions.push_back({static_cast<int>(x), static_cast<int>(y)});
                    numSafes++;
                    break;
                case spy::scenario::FieldStateEnum::FIREPLACE:
                    numFireplaces++;
                    break;
            }

        }
    }

    // match
    midChipsPerRoulette = config.getMinChipsRoulette() + config.getMaxChipsRoulette();
    midChipsPerRoulette /= 2;

    midChance = config.getGrappleHitChance() + config.getSpySuccessChance() + config.getMirrorSwapChance() +
                config.getBowlerBladeHitChance() + config.getLaserCompactHitChance() + config.getCocktailDodgeChance();
    midChance /= 6;

    midHitChanceWithDamage = config.getBowlerBladeHitChance() + config.getLaserCompactHitChance();
    midHitChanceWithDamage /= 2;

    midRangeWithDamage = config.getMoledieRange() + config.getBowlerBladeRange() + config.getMothballPouchRange();
    midRangeWithDamage /= (3 * maxPlayingFieldDim);
    midRangeWithDamage = std::min(1.0, midRangeWithDamage);

    midDamage = config.getGasGlossDamage() + config.getBowlerBladeDamage() + config.getMothballPouchDamage() +
                config.getRocketPenDamage();
    midDamage /= (4 * 100);
    midDamage = std::min(1.0, midDamage);

    // character
    for (const auto &info: characterConfig) {
        numCharacter++;
        for (const auto &p: info.getFeatures()) {
            switch (p) {
                case spy::character::PropertyEnum::NIMBLENESS:
                    numNimbleness++;
                    break;
                case spy::character::PropertyEnum::SLUGGISHNESS:
                    numSluggishness++;
                    break;
                case spy::character::PropertyEnum::PONDEROUSNESS:
                    numPonderousness++;
                    break;
                case spy::character::PropertyEnum::SPRYNESS:
                    numSpryness++;
                    break;
                case spy::character::PropertyEnum::AGILITY:
                    numAgility++;
                    break;
                case spy::character::PropertyEnum::LUCKY_DEVIL:
                    numLuckyDevil++;
                    break;
                case spy::character::PropertyEnum::JINX:
                    numJinx++;
                    break;
                case spy::character::PropertyEnum::CLAMMY_CLOTHES:
                    numClammyClothes++;
                    break;
                case spy::character::PropertyEnum::CONSTANT_CLAMMY_CLOTHES:
                    numConstantClammyClothes++;
                    break;
                case spy::character::PropertyEnum::ROBUST_STOMACH:
                    numRobustStomach++;
                    break;
                case spy::character::PropertyEnum::TOUGHNESS:
                    numToughness++;
                    break;
                case spy::character::PropertyEnum::BABYSITTER:
                    numBabysitter++;
                    break;
                case spy::character::PropertyEnum::HONEY_TRAP:
                    numHoneyTrap++;
                    break;
                case spy::character::PropertyEnum::BANG_AND_BURN:
                    numBangAndBurn++;
                    break;
                case spy::character::PropertyEnum::FLAPS_AND_SEALS:
                    numFlapsAndSeals++;
                    break;
                case spy::character::PropertyEnum::TRADECRAFT:
                    numTradecraft++;
                    break;
                case spy::character::PropertyEnum::OBSERVATION:
                    numObservation++;
                    break;
            }
        }
    }

    // mixed
    maxChipsInCasino = config.getMaxChipsRoulette() * numRouletteTables;
    chipsToIpJudge = maxChipsInCasino * static_cast<double>(config.getChipsToIpFactor()) /
                     ((numSafes + numCharacter - 4) * static_cast<double>(config.getSecretToIpFactor()) +
                      maxChipsInCasino * static_cast<double>(config.getChipsToIpFactor()));

    staticVarsSet = true;
}
