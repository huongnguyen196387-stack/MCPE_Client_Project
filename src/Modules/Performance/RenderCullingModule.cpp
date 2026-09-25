#include "client/Modules/Performance/RenderCullingModule.h"

#include <imgui.h>

RenderCullingModule::RenderCullingModule()
    : Module(
        "Render Culling",
        "Bỏ qua vật thể ngoài Frustum hoặc ngoài tầm xa",
        Category::PERFORMANCE
    ) {}

void RenderCullingModule::onTick(const ModuleContext&) {
    // Culling is evaluated at render time because camera state can change
    // independently from the regular tick loop.
}

void RenderCullingModule::onRenderUI(const ModuleContext&) {
    ImGui::Checkbox("Cull Entities", &cullEntities);
    ImGui::Checkbox("Cull Tile/Block Entities", &cullTiles);
    ImGui::SliderFloat("Max Distance", &maxDistance, 16.0f, 256.0f, "%.0f blocks");
}

void RenderCullingModule::onRender3D(const ModuleContext&) {
    // Optional debug: render culled AABBs/frustum here.
}

bool RenderCullingModule::shouldRender(
    const AABB& bounds,
    const CameraState& camera
) const {
    if (!isEnabled() || !camera.valid) {
        return true;
    }

    const float maxDistanceSq = maxDistance * maxDistance;
    if (distanceSquaredToAABB(camera.position, bounds) > maxDistanceSq) {
        return false;
    }

    return camera.frustum.intersects(bounds);
}

bool RenderCullingModule::shouldRenderActor(
    std::uint64_t actorId,
    const ModuleContext& ctx
) const {
    if (!isEnabled() || !cullEntities || ctx.game == nullptr) {
        return true;
    }

    AABB bounds{};
    CameraState camera{};
    if (!ctx.game->tryGetActorAABB(actorId, bounds)) {
        return true;
    }
    if (!ctx.game->tryGetCameraState(camera)) {
        return true;
    }

    return shouldRender(bounds, camera);
}

bool RenderCullingModule::shouldRenderTile(
    std::uint64_t tileId,
    const ModuleContext& ctx
) const {
    if (!isEnabled() || !cullTiles || ctx.game == nullptr) {
        return true;
    }

    AABB bounds{};
    CameraState camera{};
    if (!ctx.game->tryGetTileAABB(tileId, bounds)) {
        return true;
    }
    if (!ctx.game->tryGetCameraState(camera)) {
        return true;
    }

    return shouldRender(bounds, camera);
}
