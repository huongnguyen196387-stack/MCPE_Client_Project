#include "client/Modules/PvP/KeystrokesHUD.h"

#include <imgui.h>

KeystrokesHUD::KeystrokesHUD()
    : Module(
        "Keystrokes HUD",
        "WASD/D-Pad và ô click trên màn hình",
        Category::PVP
    ) {}

void KeystrokesHUD::onTick(const ModuleContext&) {}

void KeystrokesHUD::drawKey(
    ImDrawList* draw,
    const ImVec2& pos,
    const ImVec2& size,
    const char* label,
    bool active,
    float scale
) {
    const ImU32 fill = active
        ? IM_COL32(70, 120, 220, 230)
        : IM_COL32(22, 22, 30, 220);

    const ImU32 border = active
        ? IM_COL32(150, 200, 255, 255)
        : IM_COL32(100, 100, 120, 255);

    draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), fill, 8.0f * scale);
    draw->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), border, 8.0f * scale, 0, 2.0f * scale);

    const ImVec2 textSize = ImGui::CalcTextSize(label);
    draw->AddText(
        ImVec2(
            pos.x + (size.x - textSize.x) * 0.5f,
            pos.y + (size.y - textSize.y) * 0.5f
        ),
        IM_COL32(245, 245, 250, 255),
        label
    );
}

void KeystrokesHUD::onRenderUI(const ModuleContext& ctx) {
    ImGui::SliderFloat("Scale", &scale_, 0.75f, 2.0f, "%.2fx");
    ImGui::Checkbox("Use D-Pad Labels", &useDPad_);
    ImGui::Checkbox("Show Clicks", &showClicks_);

    if (!isEnabled() || ctx.input == nullptr) {
        return;
    }

    const auto* io = &ImGui::GetIO();
    auto* draw = ImGui::GetForegroundDrawList();

    const float s = scale_;
    const float keyW = 52.0f * s;
    const float keyH = 44.0f * s;
    const float gap = 6.0f * s;

    const float startX = 30.0f;
    const float startY = io->DisplaySize.y - (keyH * 3.0f + gap * 2.0f) - 32.0f;

    const bool up = useDPad_ ? ctx.input->dpadUp : ctx.input->keyW;
    const bool left = useDPad_ ? ctx.input->dpadLeft : ctx.input->keyA;
    const bool down = useDPad_ ? ctx.input->dpadDown : ctx.input->keyS;
    const bool right = useDPad_ ? ctx.input->dpadRight : ctx.input->keyD;

    const char* upLabel = useDPad_ ? "UP" : "W";
    const char* leftLabel = useDPad_ ? "LEFT" : "A";
    const char* downLabel = useDPad_ ? "DOWN" : "S";
    const char* rightLabel = useDPad_ ? "RIGHT" : "D";

    drawKey(
        draw,
        ImVec2(startX + keyW + gap, startY),
        ImVec2(keyW, keyH),
        upLabel,
        up,
        s
    );

    drawKey(
        draw,
        ImVec2(startX, startY + keyH + gap),
        ImVec2(keyW, keyH),
        leftLabel,
        left,
        s
    );

    drawKey(
        draw,
        ImVec2(startX + keyW + gap, startY + keyH + gap),
        ImVec2(keyW, keyH),
        downLabel,
        down,
        s
    );

    drawKey(
        draw,
        ImVec2(startX + (keyW + gap) * 2.0f, startY + keyH + gap),
        ImVec2(keyW, keyH),
        rightLabel,
        right,
        s
    );

    if (showClicks_) {
        const float clickY = startY + (keyH + gap) * 2.0f;
        drawKey(
            draw,
            ImVec2(startX, clickY),
            ImVec2(keyW * 1.5f, keyH),
            "LMB",
            ctx.input->leftMouseDown,
            s
        );
        drawKey(
            draw,
            ImVec2(startX + keyW * 1.5f + gap, clickY),
            ImVec2(keyW * 1.5f, keyH),
            "RMB",
            ctx.input->rightMouseDown,
            s
        );
    }
}

void KeystrokesHUD::onRender3D(const ModuleContext&) {}
