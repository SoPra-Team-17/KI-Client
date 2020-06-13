//
// Created by Carolin on 17.05.2020.
//

#include "../EquipmentChoice_gen.hpp"
#include <gameLogic/generation/EquipmentChoiceGenerator.hpp>
#include <random>
#include <util/GameLogicUtils.hpp>

std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>>
EquipmentChoice_gen::random(const std::vector<spy::util::UUID> &chosenCharacterIds,
                            const std::vector<spy::gadget::GadgetEnum> &chosenGadgets) {
    std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>> m;
    for (auto id: chosenCharacterIds) {
        m[id];  //add all keys to map
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    auto g = chosenGadgets;

    while (!g.empty()) {
        for (auto id: chosenCharacterIds) {
            if (g.empty()) {
                return m;
            }
            std::uniform_int_distribution<unsigned int> randNum(0, g.size());
            unsigned int num = randNum(gen);
            for (unsigned int i = 0; i < num; i++) {
                auto gbuf = spy::util::GameLogicUtils::getRandomItemFromContainer(g);
                m.at(id).insert(*gbuf);
                g.erase(gbuf);
            }
        }
    }

    return m;
}