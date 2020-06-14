//
// Created by Carolin on 13.06.2020.
//

#include "OperationExecutor.hpp"

std::vector<spy::gameplay::State_AI>
OperationExecutor::executeGadget(const spy::gameplay::State_AI &state, const spy::gameplay::GadgetAction &op,
                                 const spy::MatchConfig &config, const libclient::LibClient &libClient) {
    spy::gameplay::State_AI s = state;
    s.operationsLeadingToState.push_back(std::make_shared<spy::gameplay::GadgetAction>(op));
    s.usedGadgets.push_back(op.getGadget());

    auto character = s.getCharacters().getByUUID(op.getCharacterId());
    character->subActionPoint();
    if (character->getMovePoints() == 0 && character->getActionPoints() == 0) {
        s.isLeafState = true;
    }

    using namespace spy::gadget;
    switch (op.getGadget()) {
        case GadgetEnum::HAIRDRYER:
            return executeHairDryer(s, op);
        case GadgetEnum::MOLEDIE:
            return executeMoleDie(s, op, config, libClient);
        case GadgetEnum::TECHNICOLOUR_PRISM:
            return executeTechnicolorPrism(s, op);
        case GadgetEnum::BOWLER_BLADE:
            return executeBowlerBlade(s, op, config, libClient);
        case GadgetEnum::POISON_PILLS:
            return executePoisonPills(s, op, config, libClient);
        case GadgetEnum::LASER_COMPACT:
            return executeLaserCompact(s, op, config, libClient);
        case GadgetEnum::ROCKET_PEN:
            return executeRocketPen(s, op, config, libClient);
        case GadgetEnum::GAS_GLOSS:
            return executeGasGloss(s, op, config, libClient);
        case GadgetEnum::MOTHBALL_POUCH:
            return executeMothballPouch(s, op, config);
        case GadgetEnum::FOG_TIN:
            return executeFogTin(s, op);
        case GadgetEnum::GRAPPLE:
            return executeGrapple(s, op, config);
        case GadgetEnum::WIRETAP_WITH_EARPLUGS:
            return executeWiretapWithEarplugs(s, op);
        case GadgetEnum::DIAMOND_COLLAR:
            return executeDiamondCollar(s, op);
        case GadgetEnum::JETPACK:
            return executeJetpack(s, op);
        case GadgetEnum::CHICKEN_FEED:
            return executeChickenFeed(s, op, libClient);
        case GadgetEnum::NUGGET:
            return executeNugget(s, op, libClient);
        case GadgetEnum::MIRROR_OF_WILDERNESS:
            return executeMirrorOfWilderness(s, op, config);
        case GadgetEnum::COCKTAIL:
            return executeCocktail(s, op, config, libClient);
        default:
            return {};
    }
}
