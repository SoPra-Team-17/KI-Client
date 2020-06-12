//
// Created by Carolin on 10.06.2020.
//

#include "evalFunctions_caro.hpp"

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
                case spy::character::PropertyEnum::PONDEROUSNESS:
                    ret += 1 - numPonderousness / numCharacter;
                    break;
                case spy::character::PropertyEnum::SPRYNESS:
                    ret -= 1 - numSpryness / numCharacter;
                    break;
                case spy::character::PropertyEnum::AGILITY:
                    ret += 1 - numAgility / numCharacter;
                    break;
                    // roulette
                case spy::character::PropertyEnum::LUCKY_DEVIL:
                    ret += numRouletteTables / numFields * (1 - numLuckyDevil / numCharacter) * chipsToIpJudge *
                           midChipsPerRoulette / maxChipsInCasion;
                    break;
                case spy::character::PropertyEnum::JINX:
                    ret -= numRouletteTables / numFields * (1 - numJinx / numCharacter) * chipsToIpJudge *
                           midChipsPerRoulette / maxChipsInCasion;
                    break;
                case spy::character::PropertyEnum::BANG_AND_BURN:
                    if (props.find(spy::character::PropertyEnum::LUCKY_DEVIL) != props.end()) {
                        ret += numRouletteTables / numFields * (1 - numBangAndBurn / numCharacter);
                    }
                    if (props.find(spy::character::PropertyEnum::JINX) != props.end()) {
                        ret -= numRouletteTables / numFields * (1 - numBangAndBurn / numCharacter);
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
                ret -= std::min(1.0, (double) config.getMoledieRange() / maxPlayingFieldDim) * 1 /
                       (double) config.getRoundLimit();
                break;
            case spy::gadget::GadgetEnum::TECHNICOLOUR_PRISM:
                ret += numRouletteTables / numFields * chipsToIpJudge * midChipsPerRoulette / maxChipsInCasion * 1 /
                       numRouletteTables;
                break;
            case spy::gadget::GadgetEnum::BOWLER_BLADE:
                ret += config.getBowlerBladeHitChance() * std::min(1.0, (double) config.getBowlerBladeDamage() / 100) *
                       std::min(1.0, (double) config.getBowlerBladeRange() / maxPlayingFieldDim) * 1 /
                       (double) config.getRoundLimit();
                break;
            case spy::gadget::GadgetEnum::MAGNETIC_WATCH:
                ret += config.getBowlerBladeHitChance() * std::min(1.0, (double) config.getBowlerBladeDamage() / 100) *
                       std::min(1.0, (double) config.getBowlerBladeRange() / maxPlayingFieldDim);
                break;
            case spy::gadget::GadgetEnum::POISON_PILLS:
                ret += std::min(1.0, (double) config.getCocktailHealthPoints() / 100) * numBarTables / numFields * 5 /
                       (double) config.getRoundLimit();
                break;
            case spy::gadget::GadgetEnum::LASER_COMPACT:
                ret += config.getLaserCompactHitChance() * numBarTables / numFields;
                break;
            case spy::gadget::GadgetEnum::ROCKET_PEN:
                ret += std::min(1.0, (double) config.getRocketPenDamage() / 100) * 1 / (double) config.getRoundLimit();
                break;
            case spy::gadget::GadgetEnum::GAS_GLOSS:
                ret += std::min(1.0, (double) config.getGasGlossDamage() / 100) * 1 / (double) config.getRoundLimit();
                break;
            case spy::gadget::GadgetEnum::MOTHBALL_POUCH:
                ret += std::min(1.0, (double) config.getMothballPouchRange() / maxPlayingFieldDim) *
                       std::min(1.0, (double) config.getMothballPouchDamage() / 100) *
                       numFireplaces / numFields * 5 / (double) config.getRoundLimit();
                break;
            case spy::gadget::GadgetEnum::FOG_TIN:
                ret += std::min(1.0, (double) config.getFogTinRange() / maxPlayingFieldDim) * 3 /
                       (double) config.getRoundLimit();
                break;
            case spy::gadget::GadgetEnum::GRAPPLE:
                ret += config.getGrappleHitChance() *
                       std::min(1.0, (double) config.getGrappleRange() / maxPlayingFieldDim);
                break;
            case spy::gadget::GadgetEnum::WIRETAP_WITH_EARPLUGS:
                ret += (1 - config.getWiretapWithEarplugsFailChance()) * 1 / (double) config.getRoundLimit();
                break;
            case spy::gadget::GadgetEnum::JETPACK:
                ret += 1 / (double) config.getRoundLimit();
                break;
            case spy::gadget::GadgetEnum::CHICKEN_FEED:
                ret += 2 / (numCharacter - 2) * 1 / (double) config.getRoundLimit();
                break;
            case spy::gadget::GadgetEnum::NUGGET:
                ret += (1 - 4 / (numCharacter - 2)) * 1 / (double) config.getRoundLimit();
                break;
            case spy::gadget::GadgetEnum::MIRROR_OF_WILDERNESS:
                ret += config.getMirrorSwapChance() * 1 / (double) config.getRoundLimit();
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

    std::set<spy::character::PropertyEnum> posProps = {spy::character::PropertyEnum::PONDEROUSNESS};
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
            posProps.insert(spy::character::PropertyEnum::SPRYNESS);
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
            posProps.insert(spy::character::PropertyEnum::SPRYNESS);
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


    // decide on character according to porperties
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
        if (props.find(spy::character::PropertyEnum::SPRYNESS) != props.end()) {
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

double evalFunctions_caro::gameOperation(const spy::gameplay::State_AI &/*s*/, const spy::util::UUID &/*characterId*/) {
    // TODO implement
    return -std::numeric_limits<double>::infinity();
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
                    numBarTables++;
                    break;
                case spy::scenario::FieldStateEnum::ROULETTE_TABLE:
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
    maxChipsInCasion = config.getMaxChipsRoulette() * numRouletteTables;
    chipsToIpJudge = maxChipsInCasion * (double) config.getChipsToIpFactor() /
                     ((numSafes + numCharacter - 4) * (double) config.getSecretToIpFactor() +
                      maxChipsInCasion * (double) config.getChipsToIpFactor());

    staticVarsSet = true;
}
