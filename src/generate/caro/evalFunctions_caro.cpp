//
// Created by Carolin on 10.06.2020.
//

#include "evalFunctions_caro.hpp"
#include <util/GameLogicUtils.hpp>
#include <datatypes/gadgets/Cocktail.hpp>


evalFunctions_caro::evalFunctions_caro(const spy::MatchConfig &config, const spy::scenario::Scenario &scenarioConfig,
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
                default:
                    break;
            }

        }
    }

    // match
    midChipsPerRoulette = config.getMinChipsRoulette() + config.getMaxChipsRoulette();
    midChipsPerRoulette /= 2.0;

    midChance = config.getGrappleHitChance() + config.getSpySuccessChance() + config.getMirrorSwapChance() +
                config.getBowlerBladeHitChance() + config.getLaserCompactHitChance() + config.getCocktailDodgeChance();
    midChance /= 6.0;

    midHitChanceWithDamage = config.getBowlerBladeHitChance() + config.getLaserCompactHitChance();
    midHitChanceWithDamage /= 2.0;

    midRangeWithDamage = config.getMoledieRange() + config.getBowlerBladeRange() + config.getMothballPouchRange();
    midRangeWithDamage /= (3.0 * maxPlayingFieldDim);
    midRangeWithDamage = std::min(1.0, midRangeWithDamage);

    midDamage = config.getGasGlossDamage() + config.getBowlerBladeDamage() + config.getMothballPouchDamage() +
                config.getRocketPenDamage();
    midDamage /= (4.0 * 100.0);
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
                default:
                    break;
            }
        }
    }

    // mixed
    maxChipsInCasino = config.getMaxChipsRoulette() * numRouletteTables;
    maxIpInCasino = (numSafes + numCharacter - 4) * static_cast<double>(config.getSecretToIpFactor()) +
                    maxChipsInCasino * static_cast<double>(config.getChipsToIpFactor());
    chipsToIpJudge = maxChipsInCasino * static_cast<double>(config.getChipsToIpFactor()) / maxIpInCasino;
    secretsToIpJudge =
            (numSafes + numCharacter - 4) * static_cast<double>(config.getSecretToIpFactor()) / maxIpInCasino;
}

double evalFunctions_caro::itemChoice(const std::variant<const spy::util::UUID, const spy::gadget::GadgetEnum> &offer,
                                      const spy::MatchConfig &config,
                                      const std::vector<spy::character::CharacterInformation> &characterConfig) const {
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
                    ret += 1.0 - numNimbleness / numCharacter;
                    break;
                case spy::character::PropertyEnum::SLUGGISHNESS:
                    ret -= 1.0 - numSluggishness / numCharacter;
                    break;
                case spy::character::PropertyEnum::SPRYNESS:
                    ret += 1.0 - numSpryness / numCharacter;
                    break;
                case spy::character::PropertyEnum::PONDEROUSNESS:
                    ret -= 1.0 - numPonderousness / numCharacter;
                    break;
                case spy::character::PropertyEnum::AGILITY:
                    ret += 1.0 - numAgility / numCharacter;
                    break;
                    // roulette
                case spy::character::PropertyEnum::LUCKY_DEVIL:
                    ret += numRouletteTables / numFields * (1.0 - numLuckyDevil / numCharacter) * chipsToIpJudge *
                           midChipsPerRoulette / maxChipsInCasino;
                    break;
                case spy::character::PropertyEnum::JINX:
                    ret -= numRouletteTables / numFields * (1.0 - numJinx / numCharacter) * chipsToIpJudge *
                           midChipsPerRoulette / maxChipsInCasino;
                    break;
                case spy::character::PropertyEnum::BANG_AND_BURN:
                    if (props.find(spy::character::PropertyEnum::LUCKY_DEVIL) != props.end()) {
                        ret -= numRouletteTables / numFields * (1.0 - numBangAndBurn / numCharacter);
                    }
                    if (props.find(spy::character::PropertyEnum::JINX) != props.end()) {
                        ret += numRouletteTables / numFields * (1.0 - numBangAndBurn / numCharacter);
                    }
                    break;
                    // chance
                case spy::character::PropertyEnum::CLAMMY_CLOTHES:
                    ret -= midChance * (1.0 - numClammyClothes / numCharacter);
                    break;
                case spy::character::PropertyEnum::CONSTANT_CLAMMY_CLOTHES:
                    ret -= midChance * (1.0 - numConstantClammyClothes / numCharacter);
                    break;
                case spy::character::PropertyEnum::TRADECRAFT:
                    ret += midChance * (1.0 - numTradecraft / numCharacter);
                    break;
                    // HP
                case spy::character::PropertyEnum::ROBUST_STOMACH:
                    ret += static_cast<double>(config.getCocktailHealthPoints()) / maxHealthPoints * numBarTables /
                           numFields *
                           (1.0 - numRobustStomach / numCharacter);
                    break;
                case spy::character::PropertyEnum::TOUGHNESS:
                    ret += midDamage * (1.0 - numToughness / numCharacter);
                    break;
                    // protection
                case spy::character::PropertyEnum::BABYSITTER:
                    ret += midHitChanceWithDamage * config.getBabysitterSuccessChance() *
                           (1.0 - numBabysitter / numCharacter);
                    break;
                case spy::character::PropertyEnum::HONEY_TRAP:
                    ret += config.getHoneyTrapSuccessChance() * (1.0 - numHoneyTrap / numCharacter);
                    break;
                    // safes
                case spy::character::PropertyEnum::FLAPS_AND_SEALS:
                    ret += numSafes / numFields * (1.0 - numFlapsAndSeals / numCharacter);
                    break;
                    // finding out faction
                case spy::character::PropertyEnum::OBSERVATION:
                    ret += config.getObservationSuccessChance() * (1.0 - numObservation / numCharacter);
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
                ret += midChance * (1.0 - config.getCocktailDodgeChance()) * numBarTables / numFields;
                break;
            case spy::gadget::GadgetEnum::MOLEDIE:
                ret -= std::min(1.0, static_cast<double>(config.getMoledieRange() / maxPlayingFieldDim) * 1) /
                       static_cast<double>(config.getRoundLimit());
                break;
            case spy::gadget::GadgetEnum::TECHNICOLOUR_PRISM:
                ret += numRouletteTables / numFields * chipsToIpJudge * midChipsPerRoulette / maxChipsInCasino * 1.0 /
                       numRouletteTables;
                break;
            case spy::gadget::GadgetEnum::BOWLER_BLADE:
                ret += config.getBowlerBladeHitChance() *
                       std::min(1.0, static_cast<double>(config.getBowlerBladeDamage() / maxHealthPoints)) *
                       std::min(1.0, static_cast<double>(config.getBowlerBladeRange() / maxPlayingFieldDim) * 1) /
                       static_cast<double>(config.getRoundLimit());
                break;
            case spy::gadget::GadgetEnum::MAGNETIC_WATCH:
                ret += config.getBowlerBladeHitChance() *
                       std::min(1.0, static_cast<double>(config.getBowlerBladeDamage() / maxHealthPoints)) *
                       std::min(1.0, static_cast<double>(config.getBowlerBladeRange() / maxPlayingFieldDim));
                break;
            case spy::gadget::GadgetEnum::POISON_PILLS:
                ret += std::min(1.0,
                                static_cast<double>(config.getCocktailHealthPoints() / maxHealthPoints) * numBarTables /
                                numFields *
                                5) /
                       static_cast<double>(config.getRoundLimit());
                break;
            case spy::gadget::GadgetEnum::LASER_COMPACT:
                ret += config.getLaserCompactHitChance() * numBarTables / numFields;
                break;
            case spy::gadget::GadgetEnum::ROCKET_PEN:
                ret += std::min(1.0, static_cast<double>(config.getRocketPenDamage() / maxHealthPoints) * 1.0 /
                                     static_cast<double>(config.getRoundLimit()));
                break;
            case spy::gadget::GadgetEnum::GAS_GLOSS:
                ret += std::min(1.0, static_cast<double>(config.getGasGlossDamage() / maxHealthPoints) * 1.0 /
                                     static_cast<double>(config.getRoundLimit()));
                break;
            case spy::gadget::GadgetEnum::MOTHBALL_POUCH:
                ret += std::min(1.0, static_cast<double>(config.getMothballPouchRange() / maxPlayingFieldDim)) *
                       std::min(1.0, static_cast<double>(config.getMothballPouchDamage() / maxHealthPoints)) *
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
                ret += (1.0 - config.getWiretapWithEarplugsFailChance()) * 1.0 /
                       static_cast<double>(config.getRoundLimit());
                break;
            case spy::gadget::GadgetEnum::JETPACK:
                ret += 1.0 / static_cast<double>(config.getRoundLimit());
                break;
            case spy::gadget::GadgetEnum::CHICKEN_FEED:
                ret += 2 / (numCharacter - 2) * 1.0 / static_cast<double>(config.getRoundLimit());
                break;
            case spy::gadget::GadgetEnum::NUGGET:
                ret += (1.0 - 4 / (numCharacter - 2)) * 1.0 / static_cast<double>(config.getRoundLimit());
                break;
            case spy::gadget::GadgetEnum::MIRROR_OF_WILDERNESS:
                ret += config.getMirrorSwapChance() * 1.0 / static_cast<double>(config.getRoundLimit());
                break;
            case spy::gadget::GadgetEnum::POCKET_LITTER:
                ret += config.getObservationSuccessChance() * numObservation / numCharacter;
                break;
            case spy::gadget::GadgetEnum::ANTI_PLAGUE_MASK:
                ret += 10 / maxHealthPoints * (1.0 - numBarSeats / numFields);
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
                                                    const std::vector<spy::character::CharacterInformation> &characterConfig) const {

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
                double val = itemChoice(charId, config, characterConfig);
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
                                         const std::vector<spy::character::CharacterInformation> &characterConfig,
                                         const libclient::LibClient &libClient) {
    double retVal = 0;
    double val = 0;

    numMyChar = static_cast<double>(libClient.getMyFactionList().size());
    numEnemyChar = static_cast<double>(libClient.getEnemyFactionList().size());
    numUnknownChar = static_cast<double>(libClient.getUnknownFactionList().size());
    gadgetNullopt = 1.0 / (numCharacter - numMyChar);
    enemyNullopt = std::min(1.0, (std::max(0.0, 4 - numEnemyChar) + std::max(0.0, 3 - numEnemyChar) +
                                  std::max(0.0, 2 - numEnemyChar)) / (3 * numUnknownChar));
    npcNullopt = 1.0 - enemyNullopt;

    // modify nullopts in state
    modifyNulloptsInState(s);

    // collected gadgets that were not used
    auto used = s.usedGadgets;
    for (auto &gad: s.collectedGadgets) {
        auto g = std::find(used.begin(), used.end(), gad);
        if (g != used.end()) {
            used.erase(g);
            retVal += itemChoice(gad, config, characterConfig);
            if (gad == spy::gadget::GadgetEnum::DIAMOND_COLLAR) {
                retVal += config.getCatIp() / maxIpInCasino * winningReason;
            } else if (gad == spy::gadget::GadgetEnum::COCKTAIL) {
                retVal += static_cast<double>(config.getCocktailHealthPoints()) / maxHealthPoints * winningReason;
            }
        }
    }

    // health points
    val = evalHp(s, characterId, libClient);
    if (val == -std::numeric_limits<double>::infinity()) {
        return val;
    } else {
        retVal += val;
    }

    // movement
    val = evalPosition(start, s, characterId, config, libClient);
    if (val == -std::numeric_limits<double>::infinity()) {
        return val;
    } else {
        retVal += val;
    }

    // gadget
    val = evalUsedGadgets(s, config, libClient);
    if (val == -std::numeric_limits<double>::infinity()) {
        return val;
    } else {
        retVal += val;
    }

    // gamble
    retVal += s.chipDiff * chipsToIpJudge * winningReason;

    // properties
    val = evalUsedProperties(s, libClient);
    if (val == -std::numeric_limits<double>::infinity()) {
        return val;
    } else {
        retVal += val;
    }

    // spy
    val = evalSpy(s, libClient);
    if (val == -std::numeric_limits<double>::infinity()) {
        return val;
    } else {
        retVal += val;
    }

    return retVal * s.stateChance;
}

void evalFunctions_caro::modifyNulloptsInState(spy::gameplay::State_AI &s) {
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
}

double evalFunctions_caro::evalHp(const spy::gameplay::State_AI &s,
                                  const spy::util::UUID &characterId,
                                  const libclient::LibClient &libClient) {
    double retVal = 0;
    auto myCharacter = s.getCharacters().findByUUID(characterId);

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
    retVal += (myHp - enemyHp) / maxHealthPoints * winningReason;

    return retVal;
}

double evalFunctions_caro::evalPosition(const spy::gameplay::State_AI &start,
                                        const spy::gameplay::State_AI &s,
                                        const spy::util::UUID &characterId,
                                        const spy::MatchConfig &config,
                                        const libclient::LibClient &libClient) {
    double retVal = 0;
    auto myCharacter = s.getCharacters().findByUUID(characterId);

    spy::util::Point startPoint = start.getCharacters().findByUUID(characterId)->getCoordinates().value();
    spy::util::Point endPoint = myCharacter->getCoordinates().value();
    if (endPoint != startPoint) {
        auto endField = s.getMap().getField(endPoint);

        const auto &openedSafes = libClient.getOpenedSafes();
        const auto &triedSafes = libClient.getTriedSafes();
        const auto &safeCombis = libClient.getSafeCombinations();

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
                retVal += winningReason;
            } else if (!inStart && inEnd) {
                return -std::numeric_limits<double>::infinity(); // do not walk to janitor
            }
        }

        if (s.getCatCoordinates().has_value() &&
            myCharacter->hasGadget(spy::gadget::GadgetEnum::DIAMOND_COLLAR)) {
            auto startDistToCat = spy::gameplay::Movement::getMoveDistance(s.getCatCoordinates().value(), startPoint);
            auto endDistToCat = spy::gameplay::Movement::getMoveDistance(s.getCatCoordinates().value(), endPoint);
            retVal += (startDistToCat - endDistToCat) / maxPlayingFieldDim * winningReason;
        }

        if (myCharacter->hasProperty(spy::character::PropertyEnum::CLAMMY_CLOTHES) &&
            !myCharacter->hasProperty(spy::character::PropertyEnum::CONSTANT_CLAMMY_CLOTHES)) {

            bool endFireplace = endField.getFieldState() == spy::scenario::FieldStateEnum::FIREPLACE;
            retVal += endFireplace ? midChance : -midChance;
        }

        bool endBarseat = endField.getFieldState() == spy::scenario::FieldStateEnum::BAR_SEAT;
        double value = 1.0 - static_cast<double>(myCharacter->getHealthPoints()) / maxHealthPoints;
        retVal += endBarseat ? value : -value;

        for (const auto &p: safePositions) {
            auto safeIndex = libClient.safePosToIndex(s, p);
            if (openedSafes.find(safeIndex) != openedSafes.end() && triedSafes.find(safeIndex) != triedSafes.end() &&
                triedSafes.at(safeIndex) == static_cast<int>(safeCombis.size())) {
                continue; // getting close to already opened tried safes is not useful
            }
            auto startDist = spy::gameplay::Movement::getMoveDistance(p, startPoint);
            auto endDist = spy::gameplay::Movement::getMoveDistance(p, endPoint);
            double val = 1.0 / (numSafes - static_cast<double>(openedSafes.size())) * winningReason;
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
                double val = 18.0 / 37.0;
                if (myCharacter->hasProperty(spy::character::PropertyEnum::LUCKY_DEVIL)) {
                    val = 32.0 / 37.0;
                } else if (myCharacter->hasProperty(spy::character::PropertyEnum::JINX)) {
                    val = 13.0 / 37.0;
                }
                val = roulette.isInverted().value() ? (1 - val) : val;
                retVal += endDist < startDist ? val : -val;
            }
        }

        for (const auto &p: bartablePositions) {
            auto bar = s.getMap().getField(p);
            if (bar.getGadget().has_value()) {
                auto cocktail = std::dynamic_pointer_cast<spy::gadget::Cocktail>(bar.getGadget().value())->isPoisoned();
                double val = static_cast<double>(config.getCocktailHealthPoints()) / maxHealthPoints * winningReason;
                retVal += cocktail ? val : -val;
            }
        }
    }

    return retVal;
}

double evalFunctions_caro::evalSpy(const spy::gameplay::State_AI &s,
                                   const libclient::LibClient &libClient) {
    double retVal = 0;

    const auto &getCombisFromNpcs = libClient.getCombinationsFromNpcs();
    const auto &openedSafes = libClient.getOpenedSafes();
    const auto &triedSafes = libClient.getTriedSafes();
    const auto &safeCombis = libClient.getSafeCombinations();

    for (const auto &pair: s.spyResult) {
        if (pair.second.value() <= unsureLimit) {
            return -std::numeric_limits<double>::infinity(); // to unsure to spy out
        } else {
            if (getCombisFromNpcs.find(pair.first) != getCombisFromNpcs.end()) {
                retVal += (pair.second.value() - unsureLimit) * 0.5;
            } else {
                retVal += (pair.second.value() - unsureLimit) * secretsToIpJudge * winningReason;
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
        retVal += 1.0 / (numSafes - static_cast<double>(openedSafes.size())) * winningReason;
    }

    return retVal;
}

double evalFunctions_caro::evalUsedProperties(const spy::gameplay::State_AI &s,
                                              const libclient::LibClient &libClient) {
    double retVal = 0;

    // property observation
    for (const auto &val: s.observationResult) {
        if (val.value() <= unsureLimit) {
            return -std::numeric_limits<double>::infinity(); // to unsure to use observation
        } else {
            retVal += val.value() - unsureLimit;
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

    return retVal;
}

double evalFunctions_caro::evalUsedGadgets(const spy::gameplay::State_AI &s,
                                           const spy::MatchConfig &config,
                                           const libclient::LibClient &libClient) {
    double retVal = 0;

    for (const auto &usedG: s.usedGadgets) {
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
                            retVal += static_cast<double>(config.getCocktailHealthPoints()) / maxHealthPoints;
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
                                retVal += static_cast<double>(config.getCocktailHealthPoints()) /
                                          maxHealthPoints; // do remove poisoned cocktail hold by person of your faction
                            }
                        } else if (!pair.second) {
                            return -std::numeric_limits<double>::infinity(); // do not remove poisoned cocktail hold by person of other faction
                        } else if (pair.second) {
                            retVal += static_cast<double>(config.getCocktailHealthPoints()) /
                                      maxHealthPoints; // do remove healthy cocktail hold by person of other faction
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
                retVal += (enemyNullopt - myCounter) / numCharacter;
                break;
            }
            case spy::gadget::GadgetEnum::GRAPPLE:
                // already in collectedGadgets eval
                break;
            case spy::gadget::GadgetEnum::WIRETAP_WITH_EARPLUGS:
                retVal += (1.0 - config.getWiretapWithEarplugsFailChance()) * winningReason;
                break;
            case spy::gadget::GadgetEnum::DIAMOND_COLLAR:
                retVal += config.getCatIp() / maxIpInCasino * winningReason;
                break;
            case spy::gadget::GadgetEnum::JETPACK:
                // already in movement eval
                break;
            case spy::gadget::GadgetEnum::CHICKEN_FEED:
                if (s.chickenfeedResult.first.has_value()) {
                    if (s.chickenfeedResult.first.value() <= unsureLimit) {
                        return -std::numeric_limits<double>::infinity(); // to unsure, do not waste chicken feed
                    } else {
                        retVal += s.chickenfeedResult.first.value() - unsureLimit;
                    }
                }
                if (s.chickenfeedResult.second.has_value()) {
                    if (libClient.hasCharacterFaction(s.chickenfeedResult.second.value(),
                                                      spy::character::FactionEnum::PLAYER1) == 1) {
                        retVal += s.chickenfeedResult.first.value() * winningReason;
                    } else {
                        return -std::numeric_limits<double>::infinity(); // do not give ip to other factions
                    }
                }
                break;
            case spy::gadget::GadgetEnum::NUGGET:
                if (s.nuggetResult.has_value()) {
                    if (s.nuggetResult.value() <= unsureLimit) {
                        return -std::numeric_limits<double>::infinity(); // to unsure to use nugget
                    } else {
                        retVal += s.nuggetResult.value() - unsureLimit;
                    }
                }
                break;
            case spy::gadget::GadgetEnum::MIRROR_OF_WILDERNESS:
                if (s.mowResult.second.has_value()) {
                    if (libClient.hasCharacterFaction(s.mowResult.second.value(),
                                                      spy::character::FactionEnum::PLAYER1) == 1) {
                        retVal += s.mowResult.first * winningReason;
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
                        retVal += midChance * winningReason;
                    }
                }
                break;
            default:
                break;
        }
    }

    return retVal;
}
