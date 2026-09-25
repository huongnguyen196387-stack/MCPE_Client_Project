#pragma once

#include "client/Module.h"

#include <cstdint>
#include <unordered_map>

class ChunkUpdateOptimizer final : public Module {
public:
    ChunkUpdateOptimizer();

    float nearDistance{16.0f};
    float farDistance{96.0f};
    float fastMoveThreshold{6.0f};
    std::uint32_t baseIntervalMs{50};
    std::uint32_t maxIntervalMs{250};

    void onTick(const ModuleContext& ctx) override;
    void onRenderUI(const ModuleContext& ctx) override;
    void onRender3D(const ModuleContext& ctx) override;

    bool allowChunkUpdate(
        std::uint64_t chunkId,
        float distanceBlocks,
        std::uint64_t nowMs
    );

private:
    std::unordered_map<std::uint64_t, std::uint64_t> lastUpdateMs_;
    Vec3 lastPlayerPos_{};
    std::uint64_t lastSpeedSampleMs_{0};
    float playerSpeedBps_{0.0f};
    std::uint32_t pruneCounter_{0};
};
