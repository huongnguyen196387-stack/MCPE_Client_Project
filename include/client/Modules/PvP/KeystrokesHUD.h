#pragma once

#include "client/Module.h"

#include <imgui.h>

class KeystrokesHUD final : public Module {
public:
    KeystrokesHUD();

    void onTick(const ModuleContext& ctx) override;
    void onRenderUI(const ModuleContext& ctx) override;
    void onRender3D(const ModuleContext& ctx) override;

private:
    void drawKey(
        ImDrawList* draw,
        const ImVec2& pos,
        const ImVec2& size,
        const char* label,
        bool active,
        float scale
    );

    float scale_{1.0f};
    bool useDPad_{false};
    bool showClicks_{true};
};
