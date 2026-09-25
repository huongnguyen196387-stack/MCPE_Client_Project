#include "client/Modules/Performance/ChunkUpdateOptimizer.h"

#include <imgui.h>

#include <algorithm>
#include <cmath>

ChunkUpdateOptimizer::ChunkUpdateOptimizer()
    : Module(
        "Chunk Update Optimizer",
        "Giãn mesh update của chunk xa khi di chuyển nhanh",
        Category::PERFORMANCE
    ) {}

void ChunkUpdateOptimizer::onTick(const ModuleContext& ctx) {
    if (ctx.game == nullptr) {
        return;
    }

    Vec3 playerPos{};
    if (ctx.game->tryGetPlayerPosition(playerPos)) {
        if (lastSpeedSampleMs_ != 0 && ctx.nowMs > lastSpeedSampleMs_) {
            const float dt = static_cast<float>(ctx.nowMs - lastSpeedSampleMs_) / 1000.0f;
            if (dt > 0.0f) {
                playerSpeedBps_ = playerPos.distanceTo(lastPlayerPos_) / dt;
            }
        }
        lastPlayerPos_ = playerPos;
        lastSpeedSampleMs_ = ctx.nowMs;
    }

    if (++pruneCounter_ >= 120) {
        pruneCounter_ = 0;
        for (auto it = lastUpdateMs_.begin(); it != lastUpdateMs_.end();) {
            if (ctx.nowMs > it->second && (ctx.nowMs - it->second) > 10000) {
                it = lastUpdateMs_.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void ChunkUpdateOptimizer::onRenderUI(const ModuleContext&) {
    ImGui::SliderFloat("Near Distance", &nearDistance, 4.0f, 48.0f, "%.0f blocks");
    ImGui::SliderFloat("Far Distance", &farDistance, 32.0f, 192.0f, "%.0f blocks");
    ImGui::SliderFloat("Fast Move Threshold", &fastMoveThreshold, 1.0f, 15.0f, "%.1f blocks/s");

    int base = static_cast<int>(baseIntervalMs);
    int max = static_cast<int>(maxIntervalMs);

    if (ImGui::SliderInt("Base Interval", &base, 16, 250, "%d ms")) {
        baseIntervalMs = static_cast<std::uint32_t>(base);
    }
    if (ImGui::SliderInt("Max Interval", &max, 50, 1000, "%d ms")) {
        maxIntervalMs = static_cast<std::uint32_t>(std::max(max, base));
    }

    ImGui::Text("Estimated speed: %.2f blocks/s", playerSpeedBps_);
}

void ChunkUpdateOptimizer::onRender3D(const ModuleContext&) {
    // Optional debug visualization of deferred chunk rings.
}

bool ChunkUpdateOptimizer::allowChunkUpdate(
    std::uint64_t chunkId,
    float distanceBlocks,
    std::uint64_t nowMs
) {
    if (!isEnabled()) {
        return true;
    }

    if (distanceBlocks <= nearDistance) {
        lastUpdateMs_[chunkId] = nowMs;
        return true;
    }

    const float distanceT = std::clamp(
        (distanceBlocks - nearDistance) / std::max(1.0f, farDistance - nearDistance),
        0.0f,
        1.0f
    );

    const float speedT = std::clamp(
        playerSpeedBps_ / std::max(0.1f, fastMoveThreshold),
        0.0f,
        2.0f
    );

    const float pressure = std::clamp((distanceT + speedT) * 0.5f, 0.0f, 1.0f);
    const float intervalF = static_cast<float>(baseIntervalMs) +
                            (static_cast<float>(maxIntervalMs) -
                             static_cast<float>(baseIntervalMs)) * pressure;
    const auto intervalMs = static_cast<std::uint64_t>(std::max(1.0f, intervalF));

    const auto it = lastUpdateMs_.find(chunkId);
    if (it == lastUpdateMs_.end() || nowMs - it->second >= intervalMs) {
        lastUpdateMs_[chunkId] = nowMs;
        return true;
    }

    return false;
}
