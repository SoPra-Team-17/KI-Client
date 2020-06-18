//
// Created by Carolin on 11.06.2020.
//

#include <datatypes/gameplay/RetireAction.hpp>
#include <gameLogic/validation/ActionValidator.hpp>
#include <util/GameLogicUtils.hpp>
#include "OperationGenerator.hpp"

auto OperationGenerator::generate(const spy::gameplay::State &s, const spy::util::UUID &characterId,
                                  const spy::MatchConfig &config) -> std::vector<std::shared_ptr<spy::gameplay::BaseOperation>> {
    using namespace spy::gameplay;

    std::vector<std::shared_ptr<BaseOperation>> retOps;
    retOps.push_back(std::make_shared<RetireAction>(RetireAction(characterId)));

    auto character = s.getCharacters().findByUUID(characterId);

    auto neighbourPoints = spy::util::GameLogicUtils::getNearFieldsInDist(s, character->getCoordinates().value(), 1,
                                                                          [](const spy::util::Point &/*p*/) {
                                                                              return true;
                                                                          });
    GadgetAction drinkCocktail{false, character->getCoordinates().value(), characterId,
                               spy::gadget::GadgetEnum::COCKTAIL};
    if (ActionValidator::validateGadgetAction(s, drinkCocktail, config)) {
        retOps.push_back(std::make_shared<GadgetAction>(drinkCocktail));
    }
    GadgetAction selfhairdryer{false, character->getCoordinates().value(), characterId,
                               spy::gadget::GadgetEnum::HAIRDRYER};
    if (ActionValidator::validateGadgetAction(s, selfhairdryer, config)) {
        retOps.push_back(std::make_shared<GadgetAction>(selfhairdryer));
    }
    for (const auto &p: neighbourPoints.first) {

        if (character->getMovePoints() > 0) {
            // movement: only valid in dist = 1
            Movement movement{false, p, characterId, character->getCoordinates().value()};
            if (ActionValidator::validateMovement(s, movement)) {
                retOps.push_back(std::make_shared<Movement>(movement));
            }
        }

        if (character->getActionPoints() > 0) {
            // property bang and burn: only valid in dist = 1
            PropertyAction bnb{false, p, characterId, spy::character::PropertyEnum::BANG_AND_BURN};
            if (ActionValidator::validatePropertyAction(s, bnb)) {
                retOps.push_back(std::make_shared<PropertyAction>(bnb));
            }

            // gamble: only valid in dist = 1
            GambleAction gambleTest{false, p, characterId, 0};
            if (ActionValidator::validateGambleAction(s, gambleTest)) {
                // try out different stakes as gambling is possible on that point in general
                for (auto stake = 1U; stake <= character->getChips(); stake++) {
                    GambleAction gamble{false, p, characterId, stake};
                    if (ActionValidator::validateGambleAction(s, gamble)) {
                        retOps.push_back(std::make_shared<GambleAction>(gamble));
                    }
                }
            }

            // gadget chicken feed: only valid in dist = 1
            GadgetAction chickenfeed{false, p, characterId, spy::gadget::GadgetEnum::CHICKEN_FEED};
            if (ActionValidator::validateGadgetAction(s, chickenfeed, config)) {
                retOps.push_back(std::make_shared<GadgetAction>(chickenfeed));
            }

            // gadget cocktail: only valid in dist = 1 or dist = 0 (see drinkcocktail)
            GadgetAction cocktail{false, p, characterId, spy::gadget::GadgetEnum::COCKTAIL};
            if (ActionValidator::validateGadgetAction(s, cocktail, config)) {
                retOps.push_back(std::make_shared<GadgetAction>(cocktail));
            }

            // gadget diamond collar: only valid in dist = 1
            GadgetAction diamondcollar{false, p, characterId, spy::gadget::GadgetEnum::DIAMOND_COLLAR};
            if (ActionValidator::validateGadgetAction(s, diamondcollar, config)) {
                retOps.push_back(std::make_shared<GadgetAction>(diamondcollar));
            }

            // gadget gas gloss: only valid in dist = 1 (and dist = 0 is not useful)
            GadgetAction gasgloss{false, p, characterId, spy::gadget::GadgetEnum::GAS_GLOSS};
            if (ActionValidator::validateGadgetAction(s, gasgloss, config)) {
                retOps.push_back(std::make_shared<GadgetAction>(gasgloss));
            }

            // gadget hair dryer: only valid in dist = 1 or dist = 0 (see selfhairdryer)
            GadgetAction hairdryer{false, p, characterId, spy::gadget::GadgetEnum::HAIRDRYER};
            if (ActionValidator::validateGadgetAction(s, hairdryer, config)) {
                retOps.push_back(std::make_shared<GadgetAction>(hairdryer));
            }

            // gadget mirror of wilderness: only valid in dist = 1
            GadgetAction mirrorofwilderness{false, p, characterId, spy::gadget::GadgetEnum::MIRROR_OF_WILDERNESS};
            if (ActionValidator::validateGadgetAction(s, mirrorofwilderness, config)) {
                retOps.push_back(std::make_shared<GadgetAction>(mirrorofwilderness));
            }

            // gadget nugget: only valid in dist = 1
            GadgetAction nugget{false, p, characterId, spy::gadget::GadgetEnum::NUGGET};
            if (ActionValidator::validateGadgetAction(s, nugget, config)) {
                retOps.push_back(std::make_shared<GadgetAction>(nugget));
            }

            // gadget poison pills: only valid in dist = 1 (and dist = 0 is not useful)
            GadgetAction poisonpills{false, p, characterId, spy::gadget::GadgetEnum::POISON_PILLS};
            if (ActionValidator::validateGadgetAction(s, poisonpills, config)) {
                retOps.push_back(std::make_shared<GadgetAction>(poisonpills));
            }

            // gadget wiretap with earplugs: only valid in dist = 1
            GadgetAction technicolorprism{false, p, characterId, spy::gadget::GadgetEnum::TECHNICOLOUR_PRISM};
            if (ActionValidator::validateGadgetAction(s, technicolorprism, config)) {
                retOps.push_back(std::make_shared<GadgetAction>(technicolorprism));
            }

            // gadget wiretap with earplugs: only valid in dist = 1
            GadgetAction wiretapwithearplugs{false, p, characterId, spy::gadget::GadgetEnum::WIRETAP_WITH_EARPLUGS};
            if (ActionValidator::validateGadgetAction(s, wiretapwithearplugs, config)) {
                retOps.push_back(std::make_shared<GadgetAction>(wiretapwithearplugs));
            }
        }
    }

    if (character->getActionPoints() > 0) {
        auto map = s.getMap().getMap();
        for (auto y = 0U; y < map.size(); y++) {
            for (auto x = 0U; x < map.at(y).size(); x++) {
                spy::util::Point p{static_cast<int>(x), static_cast<int>(y)};

                // spy: could be valid in dist = 2
                SpyAction spy{characterId, p};
                if (ActionValidator::validateSpyAction(s, spy)) {
                    retOps.push_back(std::make_shared<SpyAction>(spy));
                }

                // property observation: valid in line of sight free
                PropertyAction obs{false, p, characterId, spy::character::PropertyEnum::OBSERVATION};
                if (ActionValidator::validatePropertyAction(s, obs)) {
                    retOps.push_back(std::make_shared<PropertyAction>(obs));
                }

                // gadget bowler blade: valid in line of sight free
                GadgetAction bowlerblade{false, p, characterId, spy::gadget::GadgetEnum::BOWLER_BLADE};
                if (ActionValidator::validateGadgetAction(s, bowlerblade, config)) {
                    retOps.push_back(std::make_shared<GadgetAction>(bowlerblade));
                }

                // gadget fog tin: valid in line of sight free
                GadgetAction fogtin{false, p, characterId, spy::gadget::GadgetEnum::FOG_TIN};
                if (ActionValidator::validateGadgetAction(s, fogtin, config)) {
                    retOps.push_back(std::make_shared<GadgetAction>(fogtin));
                }

                // gadget grapple: valid in line of sight free
                GadgetAction grapple{false, p, characterId, spy::gadget::GadgetEnum::GRAPPLE};
                if (ActionValidator::validateGadgetAction(s, grapple, config)) {
                    retOps.push_back(std::make_shared<GadgetAction>(grapple));
                }

                // gadget jetpack: valid on whole field
                GadgetAction jetpack{false, p, characterId, spy::gadget::GadgetEnum::JETPACK};
                if (ActionValidator::validateGadgetAction(s, jetpack, config)) {
                    retOps.push_back(std::make_shared<GadgetAction>(jetpack));
                }

                // gadget laser compact: valid in line of sight free
                GadgetAction lasercompact{false, p, characterId, spy::gadget::GadgetEnum::LASER_COMPACT};
                if (ActionValidator::validateGadgetAction(s, lasercompact, config)) {
                    retOps.push_back(std::make_shared<GadgetAction>(lasercompact));
                }

                // gadget moledie: valid in line of sight free
                GadgetAction moledie{false, p, characterId, spy::gadget::GadgetEnum::MOLEDIE};
                if (ActionValidator::validateGadgetAction(s, moledie, config)) {
                    retOps.push_back(std::make_shared<GadgetAction>(moledie));
                }

                // gadget mothball pouch: valid in line of sight free
                GadgetAction mothballpouch{false, p, characterId, spy::gadget::GadgetEnum::MOTHBALL_POUCH};
                if (ActionValidator::validateGadgetAction(s, mothballpouch, config)) {
                    retOps.push_back(std::make_shared<GadgetAction>(mothballpouch));
                }

                // gadget rocket pen: valid in line of sight free
                GadgetAction rocketpen{false, p, characterId, spy::gadget::GadgetEnum::ROCKET_PEN};
                if (ActionValidator::validateGadgetAction(s, rocketpen, config)) {
                    retOps.push_back(std::make_shared<GadgetAction>(rocketpen));
                }
            }
        }
    }

    return retOps;
}
