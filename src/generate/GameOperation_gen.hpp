//
// Created by Carolin on 17.05.2020.
//

#ifndef KICLIENT_GAMEOPERATION_GEN_HPP
#define KICLIENT_GAMEOPERATION_GEN_HPP

#include <util/UUID.hpp>
#include <datatypes/gameplay/State.hpp>
#include <datatypes/matchconfig/MatchConfig.hpp>
#include <LibClient.hpp>

class GameOperation_gen {
    public:
        GameOperation_gen() = delete;

        static std::shared_ptr<spy::gameplay::BaseOperation>
        generate(unsigned int difficulty, const spy::util::UUID &characterId, const spy::gameplay::State &s,
                 const spy::MatchConfig &config, const libclient::LibClient &libClient);

    private:
        static std::shared_ptr<spy::gameplay::BaseOperation>
        random(const spy::util::UUID &characterId, const spy::gameplay::State &s, const spy::MatchConfig &config);

        static std::shared_ptr<spy::gameplay::BaseOperation>
        caro(const spy::util::UUID &characterId, const spy::gameplay::State &s, const spy::MatchConfig &config,
             const libclient::LibClient &libClient);
};

#endif //KICLIENT_GAMEOPERATION_GEN_HPP
