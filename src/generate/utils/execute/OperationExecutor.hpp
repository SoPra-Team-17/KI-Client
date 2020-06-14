//
// Created by Carolin on 11.06.2020.
//

#ifndef KICLIENT_OPERATIONEXECUTOR_HPP
#define KICLIENT_OPERATIONEXECUTOR_HPP


#include <generate/utils/State_AI.hpp>
#include <vector>
#include <datatypes/gameplay/GadgetAction.hpp>
#include <datatypes/gameplay/GambleAction.hpp>
#include <datatypes/gameplay/PropertyAction.hpp>
#include <datatypes/gameplay/SpyAction.hpp>
#include <datatypes/gameplay/RetireAction.hpp>

class OperationExecutor {
    public:
        OperationExecutor() = delete;

        static std::vector<spy::gameplay::State_AI>
        execute(const spy::gameplay::State_AI &s, std::shared_ptr<spy::gameplay::BaseOperation> op,
                const spy::MatchConfig &config, const libclient::LibClient &libClient);

        static std::vector<spy::gameplay::State_AI>
        executeRetire(const spy::gameplay::State_AI &state, const spy::gameplay::RetireAction &op);

        static std::vector<spy::gameplay::State_AI>
        executeMovement(const spy::gameplay::State_AI &state, const spy::gameplay::Movement &op);

        static std::vector<spy::gameplay::State_AI>
        executeGadget(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                      const spy::MatchConfig &config, const libclient::LibClient &libClient);

        static std::vector<spy::gameplay::State_AI>
        executeGamble(const spy::gameplay::State_AI &state, const spy::gameplay::GambleAction &op);

        static std::vector<spy::gameplay::State_AI>
        executeProperty(const spy::gameplay::State_AI &state, const spy::gameplay::PropertyAction &op,
                        const spy::MatchConfig &config, const libclient::LibClient &libClient);

        static std::vector<spy::gameplay::State_AI>
        executeSpy(const spy::gameplay::State_AI &state, const spy::gameplay::SpyAction &op,
                   const spy::MatchConfig &config, const libclient::LibClient &libClient);

    private:
        static std::vector<spy::gameplay::State_AI>
        executeObservation(const spy::gameplay::State_AI &state, const spy::gameplay::PropertyAction &op,
                           const spy::MatchConfig &config, const libclient::LibClient &libClient);

        static std::vector<spy::gameplay::State_AI>
        executeBangAndBurn(const spy::gameplay::State_AI &state, const spy::gameplay::PropertyAction &op);

        static std::vector<spy::gameplay::State_AI>
        executeWiretapWithEarplugs(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op);

        static std::vector<spy::gameplay::State_AI>
        executeJetpack(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op);

        static std::vector<spy::gameplay::State_AI>
        executeGasGloss(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                        const spy::MatchConfig &config, const libclient::LibClient &libClient);

        static std::vector<spy::gameplay::State_AI>
        executeHairDryer(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op);

        static std::vector<spy::gameplay::State_AI>
        executeGrapple(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                       const spy::MatchConfig &config);

        static std::vector<spy::gameplay::State_AI>
        executeTechnicolorPrism(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op);

        static std::vector<spy::gameplay::State_AI>
        executeBowlerBlade(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                           const spy::MatchConfig &config, const libclient::LibClient &libClient);

        static std::vector<spy::gameplay::State_AI>
        executeLaserCompact(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                            const spy::MatchConfig &config, const libclient::LibClient &libClient);

        static std::vector<spy::gameplay::State_AI>
        executeChickenFeed(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op);

        static std::vector<spy::gameplay::State_AI>
        executeMirrorOfWilderness(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                                  const spy::MatchConfig &config);

        static std::vector<spy::gameplay::State_AI>
        executeNugget(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                      const libclient::LibClient &libClient);

        static std::vector<spy::gameplay::State_AI>
        executeRocketPen(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                         const spy::MatchConfig &config, const libclient::LibClient &libClient);

        static std::vector<spy::gameplay::State_AI>
        executeMoleDie(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                       const spy::MatchConfig &config, const libclient::LibClient &libClient);

        static std::vector<spy::gameplay::State_AI>
        executeFogTin(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op);

        static std::vector<spy::gameplay::State_AI>
        executeDiamondCollar(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op);

        static std::vector<spy::gameplay::State_AI>
        executePoisonPills(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                           const spy::MatchConfig &config, const libclient::LibClient &libClient);

        static std::vector<spy::gameplay::State_AI>
        executeMothballPouch(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                             const spy::MatchConfig &config);

        static std::vector<spy::gameplay::State_AI>
        executeCocktail(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                        const spy::MatchConfig &config);
};


#endif //KICLIENT_OPERATIONEXECUTOR_HPP
