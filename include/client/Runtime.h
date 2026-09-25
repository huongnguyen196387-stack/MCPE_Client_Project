#pragma once

#include "client/GameBridge.h"
#include "client/InputState.h"
#include "client/ModuleManager.h"
#include "client/ui/ImGuiLayer.h"

#include <android/native_window.h>

class ClientRuntime {
public:
    bool initialize(ANativeWindow* window);
    void shutdown();

    void tick();
    void render();

    void setGameBridge(IGameBridge* bridge);
    void setInputState(const InputState& input);

    bool shouldRenderActor(std::uint64_t actorId) const;
    bool shouldRenderTile(std::uint64_t tileId) const;
    bool allowChunkUpdate(std::uint64_t chunkId, float distanceBlocks);

    ModuleManager& modules() { return modules_; }

private:
    class NoopGameBridge final : public IGameBridge {
    public:
        bool tryGetActorAABB(std::uint64_t, AABB&) const override { return false; }
        bool tryGetTileAABB(std::uint64_t, AABB&) const override { return false; }
        bool tryGetCameraState(CameraState&) const override { return false; }
        bool tryGetPlayerPosition(Vec3&) const override { return false; }
        bool tryGetArmorSnapshot(ArmorSnapshot&) const override { return false; }
        bool tryReadFov(float&) const override { return false; }
        bool tryWriteFov(float) override { return false; }
    };

    static std::uint64_t nowMs();

    NoopGameBridge noopBridge_{};
    IGameBridge* game_{&noopBridge_};

    InputState input_{};
    ModuleManager modules_{};
    ImGuiLayer imgui_{};

    bool initialized_{false};
};
