#include "client/Modules/PvP/CustomFOV.h"

#include <imgui.h>

#include <algorithm>

CustomFOV::CustomFOV()
    : Module(
        "Custom FOV",
        "Đọc/ghi FOV qua GameBridge",
        Category::PVP
    ) {}

void CustomFOV::onEnable(const ModuleContext& ctx) {
    if (ctx.game != nullptr) {
        float current = 90.0f;
        if (ctx.game->tryReadFov(current)) {
            originalFov_ = current;
            haveOriginal_ = true;
        }
    }
}

void CustomFOV::onDisable(const ModuleContext& ctx) {
    if (ctx.game != nullptr && haveOriginal_) {
        (void)ctx.game->tryWriteFov(originalFov_);
    }
    haveOriginal_ = false;
}

void CustomFOV::onTick(const ModuleContext& ctx) {
    if (!isEnabled() || ctx.game == nullptr) {
        return;
    }

    float requested = targetFov;

    if (clampToGameSafeRange) {
        requested = std::clamp(requested, 60.0f, 140.0f);
    } else {
        requested = std::clamp(requested, 30.0f, 170.0f);
    }

    (void)ctx.game->tryWriteFov(requested);
}

void CustomFOV::onRenderUI(const ModuleContext& ctx) {
    float minFov = clampToGameSafeRange ? 60.0f : 30.0f;
    float maxFov = clampToGameSafeRange ? 140.0f : 170.0f;

    ImGui::Checkbox("Clamp Range", &clampToGameSafeRange);
    ImGui::SliderFloat("Target FOV", &targetFov, minFov, maxFov, "%.0f");

    if (ctx.game != nullptr) {
        float current = 0.0f;
        if (ctx.game->tryReadFov(current)) {
            ImGui::Text("Game FOV: %.1f", current);
        } else {
            ImGui::TextDisabled("Game FOV: unavailable");
        }
    }
}

void CustomFOV::onRender3D(const ModuleContext&) {}
