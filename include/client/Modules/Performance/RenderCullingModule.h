#pragma once

#include "client/Module.h"

#include <cstdint>

class RenderCullingModule final : public Module {
public:
    RenderCullingModule();

    bool cullEntities{true};
    bool cullTiles{true};
    float maxDistance{64.0f};

    void onTick(const ModuleContext& ctx) override;
    void onRenderUI(const ModuleContext& ctx) override;
    void onRender3D(const ModuleContext& ctx) override;

    bool shouldRenderActor(std::uint64_t actorId, const ModuleContext& ctx) const;
    bool shouldRenderTile(std::uint64_t tileId, const ModuleContext& ctx) const;
    bool shouldRender(const AABB& bounds, const CameraState& camera) const;
};
