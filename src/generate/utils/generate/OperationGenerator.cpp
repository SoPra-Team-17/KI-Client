//
// Created by Carolin on 11.06.2020.
//

#include <datatypes/gameplay/RetireAction.hpp>
#include <gameLogic/validation/ActionValidator.hpp>
#include "OperationGenerator.hpp"

auto OperationGenerator::generate(const spy::gameplay::State &s, const spy::util::UUID &characterId,
                                  const spy::MatchConfig &config) -> std::vector<std::shared_ptr<spy::gameplay::BaseOperation>> {
    using namespace spy::gameplay;

    std::vector<std::shared_ptr<BaseOperation>> retOps;
    retOps.push_back(std::make_shared<RetireAction>(RetireAction(characterId)));

    auto character = s.getCharacters().findByUUID(characterId);

    auto map = s.getMap().getMap();
    for (auto y = 0U; y < map.size(); y++) {
        for (auto x = 0U; x < map.at(y).size(); x++) {
            spy::util::Point p {static_cast<int>(x), static_cast<int>(y)};

            Movement movement {false, p, characterId, character->getCoordinates().value()};
            if (ActionValidator::validateMovement(s, movement)) {
                retOps.push_back(std::make_shared<Movement>(movement));
            }

            SpyAction spy {characterId, p};
            if (ActionValidator::validateSpyAction(s, spy)) {
                retOps.push_back(std::make_shared<SpyAction>(spy));
            }

            PropertyAction bnb {false, p, characterId, spy::character::PropertyEnum::BANG_AND_BURN};
            PropertyAction obs {false, p, characterId, spy::character::PropertyEnum::OBSERVATION};
            if (ActionValidator::validatePropertyAction(s, bnb)) {
                retOps.push_back(std::make_shared<PropertyAction>(bnb));
            }
            if (ActionValidator::validatePropertyAction(s, obs)) {
                retOps.push_back(std::make_shared<PropertyAction>(obs));
            }

            for (auto stake = 1U; stake <= character->getChips(); stake++) {
                GambleAction gamble {false, p, characterId, stake};
                if (ActionValidator::validateGambleAction(s, gamble)) {
                    retOps.push_back(std::make_shared<GambleAction>(gamble));
                }
            }

            // not nice but have to iterate over enum values
            for (int gadgetType = 1; gadgetType < 22; gadgetType++) {
                GadgetAction gadget {false, p, characterId, spy::gadget::GadgetEnum(gadgetType)};
                if (ActionValidator::validateGadgetAction(s, gadget, config)) {
                    retOps.push_back(std::make_shared<GadgetAction>(gadget));
                }
            }
        }
    }
    
    return retOps;
}
