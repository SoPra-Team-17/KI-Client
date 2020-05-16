//
// Created by Carolin on 16.05.2020.
//

#ifndef KICLIENT_AICALLBACK_HPP
#define KICLIENT_AICALLBACK_HPP

#include <spdlog/spdlog.h>
#include <LibClient.hpp>
#include "AI.hpp"

class AICallback : public libclient::Callback {
    public:
        explicit AICallback(AI &ai);

        void onHelloReply() override;

        void onGameStarted() override;

        void onRequestItemChoice() override;

        void onRequestEquipmentChoice() override;

        void onGameStatus() override;

        void onRequestGameOperation() override;

        void onStatistics() override;

        void onGameLeft() override;

        void onGamePause() override;

        void onMetaInformation() override;

        void onStrike() override;

        void onError() override;

        void onReplay() override;

        void connectionLost() override;

        void wrongDestination() override;

    private:
        AI &ai;
};


#endif //KICLIENT_AICALLBACK_HPP
