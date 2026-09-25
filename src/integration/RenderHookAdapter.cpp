#include "client/integration/RenderHookAdapter.h"

bool RenderHookAdapter::shouldCallActorRenderer(
    const ClientRuntime& runtime,
    std::uint64_t actorId
) {
    return runtime.shouldRenderActor(actorId);
}

bool RenderHookAdapter::shouldCallTileRenderer(
    const ClientRuntime& runtime,
    std::uint64_t tileId
) {
    return runtime.shouldRenderTile(tileId);
}
