# MCPE Client Project — Android NDK + Dear ImGui

Framework C++20 dành cho Android NDK, cung cấp:

- `Module` abstract base class: name, description, enabled, category, `onTick()`, `onRenderUI()`, `onRender3D()`.
- `ModuleManager`: đăng ký module, bật/tắt, tick và render theo category.
- Sidebar ImGui với các tab `PvP`, `Performance`, `Visuals`, `Settings`.
- Performance:
  - `RenderCullingModule`: Frustum + AABB + max distance.
  - `ChunkUpdateOptimizer`: trì hoãn mesh update xa theo khoảng cách và tốc độ di chuyển.
- PvP:
  - `CPSCounter`
  - `KeystrokesHUD`
  - `ArmorStatus`
  - `CustomFOV`
- API bridge để host/loader tích hợp Bedrock-specific data.
- GitHub Actions build native `.so` cho `arm64-v8a` và `x86_64`.

## Quan trọng về Bedrock integration

Project này **không chứa offset memory, byte patch, signature pattern hoặc detour dành cho một build Bedrock cụ thể**. Các phần đó thay đổi theo phiên bản game và ABI.

Thay vào đó:

- `IGameBridge` là abstraction để host cung cấp Actor AABB, camera/frustum, armor snapshot, FOV...
- `RenderHookAdapter` là điểm nối logic culling vào hook/detour do host của bạn quản lý.
- `mcpe_client_should_render_actor()` là hàm kiểm tra nhanh có thể gọi từ adapter trước khi vào renderer gốc.
- `mcpe_client_allow_chunk_update()` là scheduler gate có thể dùng trước khi enqueue/rebuild mesh.

Xem `docs/BedrockIntegration.md`.

## Build bằng Android NDK

Ví dụ Linux/macOS:

```bash
export ANDROID_NDK_ROOT=/path/to/android-ndk-r30

cmake -S . -B build \
  -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK_ROOT/build/cmake/android.toolchain.cmake" \
  -DANDROID_ABI=arm64-v8a \
  -DANDROID_PLATFORM=android-24 \
  -DCMAKE_BUILD_TYPE=Release

cmake --build build --config Release
```

Output:

```text
build/libmcpe_client.so
```

## Dear ImGui

CMake fetch Dear ImGui lúc configure từ repository chính thức. Project dùng Android + OpenGL ES 3 backend. Luồng tích hợp chuẩn là `CreateContext()`, backend init, `NewFrame()`, `Render()` và backend render draw data.

## GitHub Actions

Workflow tại `.github/workflows/android.yml`:

- setup Java
- setup Android SDK
- cài NDK LTS
- build 2 ABI
- upload `.so` artifacts

## Hướng tích hợp host

Một host Android có OpenGL/EGL context có thể:

1. Load `libmcpe_client.so`.
2. Gọi `mcpe_client_init(ANativeWindow*)`.
3. Mỗi frame:
   - cập nhật input với `mcpe_client_set_input_state(...)`
   - gọi `mcpe_client_tick()`
   - bắt đầu frame renderer/game
   - gọi `mcpe_client_render()`
4. Adapter renderer gọi `mcpe_client_should_render_actor(...)` trước renderer gốc.
5. Adapter chunk gọi `mcpe_client_allow_chunk_update(...)` trước rebuild.

Chi tiết trong `docs/BedrockIntegration.md`.
