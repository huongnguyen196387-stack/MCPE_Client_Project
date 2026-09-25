#pragma once

#include "client/Types.h"

#include <array>
#include <cstdint>
#include <string>

enum class ArmorSlot : std::uint8_t {
    Helmet = 0,
    Chestplate,
    Leggings,
    Boots
};

struct ItemSnapshot {
    bool valid{false};
    std::string name;
    std::string shortLabel;
    int durability{0};
    int maxDurability{0};
};

struct ArmorSnapshot {
    std::array<ItemSnapshot, 4> armor{};
    ItemSnapshot mainHand{};
    ItemSnapshot offHand{};
};

class IGameBridge {
public:
    virtual ~IGameBridge() = default;

    virtual bool tryGetActorAABB(std::uint64_t actorId, AABB& out) const = 0;
    virtual bool tryGetTileAABB(std::uint64_t tileId, AABB& out) const = 0;
    virtual bool tryGetCameraState(CameraState& out) const = 0;
    virtual bool tryGetPlayerPosition(Vec3& out) const = 0;

    virtual bool tryGetArmorSnapshot(ArmorSnapshot& out) const = 0;

    virtual bool tryReadFov(float& outFov) const = 0;
    virtual bool tryWriteFov(float fov) = 0;
};
