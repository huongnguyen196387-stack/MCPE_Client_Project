#pragma once

#include "client/Module.h"

#include <cstdint>
#include <deque>

class CPSCounter final : public Module {
public:
    CPSCounter();

    void onTick(const ModuleContext& ctx) override;
    void onRenderUI(const ModuleContext& ctx) override;
    void onRender3D(const ModuleContext& ctx) override;

    int getLeftCPS(std::uint64_t nowMs);
    int getRightCPS(std::uint64_t nowMs);

private:
    void prune(std::uint64_t nowMs);

    std::deque<std::uint64_t> leftClicks_;
    std::deque<std::uint64_t> rightClicks_;
    bool lastLeftDown_{false};
    bool lastRightDown_{false};

    bool showOverlay_{true};
    float overlayScale_{1.0f};
};
