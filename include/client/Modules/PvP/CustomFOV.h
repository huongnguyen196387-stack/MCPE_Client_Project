#pragma once

#include "client/Module.h"

class CustomFOV final : public Module {
public:
    CustomFOV();

    float targetFov{120.0f};
    bool clampToGameSafeRange{true};

    void onTick(const ModuleContext& ctx) override;
    void onRenderUI(const ModuleContext& ctx) override;
    void onRender3D(const ModuleContext& ctx) override;

protected:
    void onEnable(const ModuleContext& ctx) override;
    void onDisable(const ModuleContext& ctx) override;

private:
    bool haveOriginal_{false};
    float originalFov_{90.0f};
};
