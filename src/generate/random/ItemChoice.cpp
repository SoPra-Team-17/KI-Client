//
// Created by Carolin on 17.05.2020.
//

#include "../ItemChoice.hpp"

std::variant<spy::util::UUID, spy::gadget::GadgetEnum>
ItemChoice::random(const std::vector<spy::util::UUID> &/*offeredCharacterIds*/,
                   const std::vector<spy::gadget::GadgetEnum> &/*offeredGadgets*/) {
    // TODO implement
    return std::variant<spy::util::UUID, spy::gadget::GadgetEnum>();
}