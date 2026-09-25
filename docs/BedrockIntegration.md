# Bedrock Integration Notes

## 1. Render Culling

`RenderCullingModule` thực hiện:

1. Lấy `AABB` của Actor/TileEntity từ `IGameBridge`.
2. Lấy `CameraState` gồm:
   - camera position
   - view-projection matrix
   - frustum 6 planes
3. Test khoảng cách từ camera tới AABB.
4. Test AABB với 6 planes.
5. Nếu fail bất kỳ test nào, renderer adapter bỏ qua lệnh render.

### Frustum math

Từ ma trận `M = Projection * View`, các plane được trích xuất theo convention row-major:

```text
Left   = row4 + row1
Right  = row4 - row1
Bottom = row4 + row2
Top    = row4 - row2
Near   = row4 + row3
Far    = row4 - row3
```

Sau đó normalize `plane.normal` và dùng positive-vertex AABB test.

## 2. Renderer hook point

Project cố ý không hard-code offset của một phiên bản Bedrock.

Adapter host nên có logic tương đương:

```cpp
// Pseudocode — call from your own supported hook/detour layer.
bool shouldCallOriginal =
    mcpe_client_should_render_actor(actorId);

if (!shouldCallOriginal) {
    return; // skip original ActorRenderer::render-equivalent call
}

originalActorRendererRender(...);
```

Với TileEntity, host có thể dùng API tương tự sau khi mở rộng `ClientRuntime` theo engine version đang target.

## 3. Chunk mesh update optimization

`ChunkUpdateOptimizer` không sửa bytecode game. Nó cung cấp một scheduler gate:

```cpp
if (mcpe_client_allow_chunk_update(chunkId, distanceInBlocks)) {
    rebuildOrUploadChunkMesh();
}
```

Khi người chơi di chuyển nhanh, các chunk xa được cấp interval lớn hơn; chunk gần vẫn được ưu tiên.

## 4. Armor / FOV

`ArmorStatus` và `CustomFOV` dùng `IGameBridge`:

- `tryGetArmorSnapshot(...)`
- `tryReadFov(...)`
- `tryWriteFov(...)`

Đây là chỗ để host hợp pháp/được phép của bạn kết nối với game abstraction tương ứng. Project không cung cấp memory offset, signature scan hay patch bytes.

## 5. Stability

Nên kiểm tra tất cả pointer/handle game-side trước khi truy cập và disable integration khi build/version không khớp. Tránh giữ raw pointers sang object game nếu lifecycle của chúng không được đảm bảo.
