#pragma once

#include "client/Module.h"

#include <cstdint>

class ArmorStatus final : public Module {
public:
    ArmorStatus();

    void onTick(const ModuleContext& ctx) override;
    void onRenderUI(const ModuleContext& ctx) override;
    void onRender3D(const ModuleContext& ctx) override;

private:
    ArmorSnapshot snapshot_{};
    std::uint64_t nextRefreshMs_{0};
    float scale_{1.0f};
    bool showHands_{true};
};
