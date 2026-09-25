#include "client/Modules/PvP/CPSCounter.h"

#include <imgui.h>

#include <algorithm>
#include <cstdio>

CPSCounter::CPSCounter()
    : Module(
        "CPS Counter",
        "Đếm click trong cửa sổ 1 giây",
        Category::PVP
    ) {}

void CPSCounter::prune(std::uint64_t nowMs) {
    const std::uint64_t cutoff = nowMs >= 1000 ? nowMs - 1000 : 0;

    while (!leftClicks_.empty() && leftClicks_.front() < cutoff) {
        leftClicks_.pop_front();
    }
    while (!rightClicks_.empty() && rightClicks_.front() < cutoff) {
        rightClicks_.pop_front();
    }
}

void CPSCounter::onTick(const ModuleContext& ctx) {
    if (ctx.input == nullptr) {
        return;
    }

    const auto now = ctx.nowMs;

    if (ctx.input->leftMouseDown && !lastLeftDown_) {
        leftClicks_.push_back(now);
    }
    if (ctx.input->rightMouseDown && !lastRightDown_) {
        rightClicks_.push_back(now);
    }

    lastLeftDown_ = ctx.input->leftMouseDown;
    lastRightDown_ = ctx.input->rightMouseDown;

    prune(now);
}

int CPSCounter::getLeftCPS(std::uint64_t nowMs) {
    prune(nowMs);
    return static_cast<int>(leftClicks_.size());
}

int CPSCounter::getRightCPS(std::uint64_t nowMs) {
    prune(nowMs);
    return static_cast<int>(rightClicks_.size());
}

void CPSCounter::onRenderUI(const ModuleContext& ctx) {
    ImGui::Checkbox("Show Overlay", &showOverlay_);
    ImGui::SliderFloat("Overlay Scale", &overlayScale_, 0.75f, 2.0f, "%.2fx");

    if (ctx.nowMs > 0) {
        ImGui::Text("Left CPS: %d", getLeftCPS(ctx.nowMs));
        ImGui::Text("Right CPS: %d", getRightCPS(ctx.nowMs));
    }

    if (!isEnabled() || !showOverlay_) {
        return;
    }

    auto* draw = ImGui::GetForegroundDrawList();
    const float s = overlayScale_;
    const ImVec2 pos(22.0f, 22.0f);
    const ImVec2 size(176.0f * s, 72.0f * s);

    draw->AddRectFilled(
        pos,
        ImVec2(pos.x + size.x, pos.y + size.y),
        IM_COL32(18, 18, 24, 220),
        10.0f * s
    );

    draw->AddRect(
        pos,
        ImVec2(pos.x + size.x, pos.y + size.y),
        IM_COL32(110, 110, 135, 255),
        10.0f * s,
        0,
        2.0f * s
    );

    char buffer[64]{};
    std::snprintf(buffer, sizeof(buffer), "LMB: %d CPS", getLeftCPS(ctx.nowMs));
    draw->AddText(
        ImVec2(pos.x + 14.0f * s, pos.y + 11.0f * s),
        IM_COL32(245, 245, 250, 255),
        buffer
    );

    std::snprintf(buffer, sizeof(buffer), "RMB: %d CPS", getRightCPS(ctx.nowMs));
    draw->AddText(
        ImVec2(pos.x + 14.0f * s, pos.y + 38.0f * s),
        IM_COL32(245, 245, 250, 255),
        buffer
    );
}

void CPSCounter::onRender3D(const ModuleContext&) {}
