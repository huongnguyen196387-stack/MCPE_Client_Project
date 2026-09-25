#include "client/Modules/PvP/ArmorStatus.h"

#include <imgui.h>

#include <algorithm>
#include <string>
#include <utility>

ArmorStatus::ArmorStatus()
    : Module(
        "Armor Status",
        "Hiển thị giáp/vũ khí và durability",
        Category::PVP
    ) {}

void ArmorStatus::onTick(const ModuleContext& ctx) {
    if (ctx.game == nullptr || ctx.nowMs < nextRefreshMs_) {
        return;
    }

    nextRefreshMs_ = ctx.nowMs + 100;
    ArmorSnapshot tmp{};
    if (ctx.game->tryGetArmorSnapshot(tmp)) {
        snapshot_ = std::move(tmp);
    }
}

void ArmorStatus::onRenderUI(const ModuleContext&) {
    ImGui::SliderFloat("Scale", &scale_, 0.75f, 2.0f, "%.2fx");
    ImGui::Checkbox("Show Hands", &showHands_);

    if (!isEnabled()) {
        return;
    }

    auto* draw = ImGui::GetForegroundDrawList();
    const float s = scale_;
    const float size = 58.0f * s;
    const float gap = 8.0f * s;
    const ImVec2 base(22.0f, 110.0f);

    const char* labels[6] = {
        "HEAD", "CHEST", "LEGS", "FEET", "MAIN", "OFF"
    };

    auto drawItem = [&](int index, const ItemSnapshot& item) {
        const float x = base.x + static_cast<float>(index) * (size + gap);

        draw->AddRectFilled(
            ImVec2(x, base.y),
            ImVec2(x + size, base.y + size),
            IM_COL32(24, 24, 32, 225),
            9.0f * s
        );
        draw->AddRect(
            ImVec2(x, base.y),
            ImVec2(x + size, base.y + size),
            IM_COL32(110, 110, 130, 255),
            9.0f * s,
            0,
            2.0f * s
        );

        const char* text = item.valid
            ? (item.shortLabel.empty() ? item.name.c_str() : item.shortLabel.c_str())
            : labels[index];

        ImVec2 ts = ImGui::CalcTextSize(text);
        draw->AddText(
            ImVec2(x + (size - ts.x) * 0.5f, base.y + 7.0f * s),
            IM_COL32(245, 245, 250, 255),
            text
        );

        float ratio = 0.0f;
        if (item.valid && item.maxDurability > 0) {
            ratio = std::clamp(
                static_cast<float>(item.durability) /
                static_cast<float>(item.maxDurability),
                0.0f,
                1.0f
            );
        }

        const float barY = base.y + size - 12.0f * s;
        draw->AddRectFilled(
            ImVec2(x + 6.0f * s, barY),
            ImVec2(x + size - 6.0f * s, barY + 6.0f * s),
            IM_COL32(55, 55, 65, 255),
            3.0f * s
        );
        draw->AddRectFilled(
            ImVec2(x + 6.0f * s, barY),
            ImVec2(x + 6.0f * s + (size - 12.0f * s) * ratio, barY + 6.0f * s),
            IM_COL32(100, 210, 110, 255),
            3.0f * s
        );
    };

    drawItem(0, snapshot_.armor[0]);
    drawItem(1, snapshot_.armor[1]);
    drawItem(2, snapshot_.armor[2]);
    drawItem(3, snapshot_.armor[3]);

    if (showHands_) {
        drawItem(4, snapshot_.mainHand);
        drawItem(5, snapshot_.offHand);
    }
}

void ArmorStatus::onRender3D(const ModuleContext&) {}
