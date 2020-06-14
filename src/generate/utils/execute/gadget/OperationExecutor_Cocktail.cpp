//
// Created by Carolin on 13.06.2020.
//

#include <datatypes/gadgets/Cocktail.hpp>
#include <util/GameLogicUtils.hpp>
#include "../OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeCocktail(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                                   const spy::MatchConfig &config, const libclient::LibClient &libClient) {

    std::vector<spy::gameplay::State_AI> honeyStates;
    spy::gameplay::State_AI sSuccess = state;

    auto sourceChar = sSuccess.getCharacters().getByUUID(op.getCharacterId());
    auto &targetField = sSuccess.getMap().getField(op.getTarget());
    bool drink = (sourceChar->getCoordinates().value() == op.getTarget());

    if (targetField.getFieldState() == spy::scenario::FieldStateEnum::BAR_TABLE) {
        // take the cocktail from the bar table
        sourceChar->addGadget(targetField.getGadget().value());
        targetField.removeGadget();
        sSuccess.collectedGadgets.push_back(op.getGadget());
    } else {
        // remove the cocktail as it was used
        sourceChar->removeGadget(spy::gadget::GadgetEnum::COCKTAIL);

        if (drink) {
            auto configHP = config.getCocktailHealthPoints();
            // check if cocktail is poisoned
            auto cocktail = std::dynamic_pointer_cast<spy::gadget::Cocktail>(
                    sourceChar->getGadget(spy::gadget::GadgetEnum::COCKTAIL).value());
            if (!cocktail->isPoisoned()) {
                auto cocktailHP = sourceChar->hasProperty(spy::character::PropertyEnum::ROBUST_STOMACH) ? 2 * configHP
                                                                                                        : configHP;
                sourceChar->addHealthPoints(cocktailHP);
                sSuccess.addDamage(*sourceChar, -cocktailHP); // negative damage -> add hp
            } else {
                // cocktail is poisoned
                auto cocktailDamage = sourceChar->hasProperty(spy::character::PropertyEnum::ROBUST_STOMACH) ? 0.5 * configHP
                                                                                                            : configHP;
                sourceChar->subHealthPoints(cocktailDamage);
                sSuccess.addDamage(*sourceChar, cocktailDamage);
            }
        } else { // pour the cocktail over another character
            // honey trap
            auto honeyTrapResult = sSuccess.handleHoneyTrap(op, config, libClient);
            honeyStates = honeyTrapResult.first;
            if (honeyTrapResult.second) {
                return honeyStates;
            }

            if (config.getCocktailDodgeChance() == 1) {
                return {};
            }
            sSuccess.modStateChance(*sourceChar, 1 - config.getCocktailDodgeChance());
            auto targetChar = spy::util::GameLogicUtils::getInCharacterSetByCoordinates(sSuccess.getCharacters(), op.getTarget());
            if (targetChar->hasProperty(spy::character::PropertyEnum::CONSTANT_CLAMMY_CLOTHES) ||
                targetChar->hasProperty(spy::character::PropertyEnum::CLAMMY_CLOTHES)) {
                return {}; // action does not change anything
            } else {
                targetChar->addProperty(spy::character::PropertyEnum::CLAMMY_CLOTHES);
                sSuccess.addedClammyClothes.insert(targetChar->getCharacterId());
            }
        }
    }

    honeyStates.push_back(sSuccess);
    return honeyStates;
}
