#pragma once

#include "client/Runtime.h"

class RenderHookAdapter {
public:
    static bool shouldCallActorRenderer(const ClientRuntime& runtime, std::uint64_t actorId);
    static bool shouldCallTileRenderer(const ClientRuntime& runtime, std::uint64_t tileId);
};
