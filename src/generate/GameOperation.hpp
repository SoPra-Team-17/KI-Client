//
// Created by Carolin on 17.05.2020.
//

#ifndef KICLIENT_GAMEOPERATION_HPP
#define KICLIENT_GAMEOPERATION_HPP

#include <util/UUID.hpp>
#include <datatypes/gameplay/State.hpp>
#include <datatypes/matchconfig/MatchConfig.hpp>

class GameOperation {
    public:
        GameOperation() = delete;

        static auto generate(unsigned int difficulty, const spy::util::UUID &characterId, const spy::gameplay::State &s,
                             const spy::MatchConfig &config) -> std::shared_ptr<spy::gameplay::BaseOperation>;

    private:
        static auto random(const spy::util::UUID &characterId, const spy::gameplay::State &s,
                           const spy::MatchConfig &config) -> std::shared_ptr<spy::gameplay::BaseOperation>;
};

#endif //KICLIENT_GAMEOPERATION_HPP
