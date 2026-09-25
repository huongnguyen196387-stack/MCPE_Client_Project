#include "client/Runtime.h"

#include "client/Modules/Performance/ChunkUpdateOptimizer.h"
#include "client/Modules/Performance/RenderCullingModule.h"

#include <backends/imgui_impl_android.h>

#include <chrono>

namespace {
ClientRuntime* g_runtime = nullptr;
}

std::uint64_t ClientRuntime::nowMs() {
    const auto now = std::chrono::steady_clock::now().time_since_epoch();
    return static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now).count()
    );
}

bool ClientRuntime::initialize(ANativeWindow* window) {
    if (initialized_) {
        return true;
    }

    if (!imgui_.initialize(window)) {
        return false;
    }

    input_.nowMs = nowMs();
    initialized_ = true;
    return true;
}

void ClientRuntime::shutdown() {
    if (!initialized_) {
        return;
    }

    imgui_.shutdown();
    initialized_ = false;
}

void ClientRuntime::setGameBridge(IGameBridge* bridge) {
    game_ = bridge != nullptr ? bridge : &noopBridge_;
}

void ClientRuntime::setInputState(const InputState& input) {
    input_ = input;
    if (input_.nowMs == 0) {
        input_.nowMs = nowMs();
    }
}

void ClientRuntime::tick() {
    if (!initialized_) {
        return;
    }

    input_.nowMs = nowMs();

    ModuleContext ctx{
        .game = game_,
        .input = &input_,
        .nowMs = input_.nowMs
    };

    modules_.onTick(ctx);
}

void ClientRuntime::render() {
    if (!initialized_) {
        return;
    }

    ModuleContext ctx{
        .game = game_,
        .input = &input_,
        .nowMs = input_.nowMs
    };

    imgui_.newFrame();
    modules_.renderMenu(ctx);
    modules_.onRender3D(ctx);
    imgui_.render();
}

bool ClientRuntime::shouldRenderActor(std::uint64_t actorId) const {
    const auto* module = modules_.get<RenderCullingModule>();
    if (module == nullptr) {
        return true;
    }

    ModuleContext ctx{
        .game = game_,
        .input = &input_,
        .nowMs = input_.nowMs
    };

    return module->shouldRenderActor(actorId, ctx);
}

bool ClientRuntime::shouldRenderTile(std::uint64_t tileId) const {
    const auto* module = modules_.get<RenderCullingModule>();
    if (module == nullptr) {
        return true;
    }

    ModuleContext ctx{
        .game = game_,
        .input = &input_,
        .nowMs = input_.nowMs
    };

    return module->shouldRenderTile(tileId, ctx);
}

bool ClientRuntime::allowChunkUpdate(
    std::uint64_t chunkId,
    float distanceBlocks
) {
    auto* module = modules_.get<ChunkUpdateOptimizer>();
    if (module == nullptr) {
        return true;
    }

    return module->allowChunkUpdate(chunkId, distanceBlocks, input_.nowMs);
}

// C-style entry points for a host/loader. No Bedrock-specific offsets are assumed.
extern "C" {

__attribute__((visibility("default")))
bool mcpe_client_init(ANativeWindow* window) {
    if (g_runtime == nullptr) {
        g_runtime = new ClientRuntime();
    }
    return g_runtime->initialize(window);
}

__attribute__((visibility("default")))
void mcpe_client_shutdown() {
    if (g_runtime != nullptr) {
        g_runtime->shutdown();
        delete g_runtime;
        g_runtime = nullptr;
    }
}

__attribute__((visibility("default")))
void mcpe_client_set_game_bridge(IGameBridge* bridge) {
    if (g_runtime != nullptr) {
        g_runtime->setGameBridge(bridge);
    }
}

__attribute__((visibility("default")))
void mcpe_client_tick() {
    if (g_runtime != nullptr) {
        g_runtime->tick();
    }
}

__attribute__((visibility("default")))
void mcpe_client_render() {
    if (g_runtime != nullptr) {
        g_runtime->render();
    }
}

__attribute__((visibility("default")))
void mcpe_client_set_input_state(
    bool leftMouseDown,
    bool rightMouseDown,
    bool w,
    bool a,
    bool s,
    bool d,
    bool dpadUp,
    bool dpadLeft,
    bool dpadDown,
    bool dpadRight
) {
    if (g_runtime == nullptr) {
        return;
    }

    InputState state{};
    state.leftMouseDown = leftMouseDown;
    state.rightMouseDown = rightMouseDown;
    state.keyW = w;
    state.keyA = a;
    state.keyS = s;
    state.keyD = d;
    state.dpadUp = dpadUp;
    state.dpadLeft = dpadLeft;
    state.dpadDown = dpadDown;
    state.dpadRight = dpadRight;

    g_runtime->setInputState(state);
}

__attribute__((visibility("default")))
bool mcpe_client_should_render_actor(std::uint64_t actorId) {
    return g_runtime == nullptr ? true : g_runtime->shouldRenderActor(actorId);
}

__attribute__((visibility("default")))
bool mcpe_client_should_render_tile(std::uint64_t tileId) {
    return g_runtime == nullptr ? true : g_runtime->shouldRenderTile(tileId);
}

__attribute__((visibility("default")))
bool mcpe_client_allow_chunk_update(
    std::uint64_t chunkId,
    float distanceBlocks
) {
    return g_runtime == nullptr
        ? true
        : g_runtime->allowChunkUpdate(chunkId, distanceBlocks);
}

__attribute__((visibility("default")))
bool mcpe_client_handle_input_event(AInputEvent* event) {
    if (g_runtime == nullptr || event == nullptr) {
        return false;
    }
    return ImGui_ImplAndroid_HandleInputEvent(event);
}

} // extern "C"
