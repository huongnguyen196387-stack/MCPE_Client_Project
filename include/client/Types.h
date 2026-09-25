#pragma once

#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>

struct Vec2 {
    float x{0.0f};
    float y{0.0f};
};

struct Vec3 {
    float x{0.0f};
    float y{0.0f};
    float z{0.0f};

    Vec3 operator-(const Vec3& rhs) const {
        return {x - rhs.x, y - rhs.y, z - rhs.z};
    }

    float lengthSquared() const {
        return x * x + y * y + z * z;
    }

    float distanceSquaredTo(const Vec3& rhs) const {
        return (*this - rhs).lengthSquared();
    }

    float distanceTo(const Vec3& rhs) const {
        return std::sqrt(distanceSquaredTo(rhs));
    }
};

struct AABB {
    Vec3 min{};
    Vec3 max{};

    Vec3 center() const {
        return {
            (min.x + max.x) * 0.5f,
            (min.y + max.y) * 0.5f,
            (min.z + max.z) * 0.5f
        };
    }
};

inline float distanceSquaredToAABB(const Vec3& p, const AABB& b) {
    const float dx = std::max({b.min.x - p.x, 0.0f, p.x - b.max.x});
    const float dy = std::max({b.min.y - p.y, 0.0f, p.y - b.max.y});
    const float dz = std::max({b.min.z - p.z, 0.0f, p.z - b.max.z});
    return dx * dx + dy * dy + dz * dz;
}

struct Matrix4 {
    // Row-major.
    float m[4][4]{};

    const float* operator[](std::size_t row) const {
        return m[row];
    }

    float* operator[](std::size_t row) {
        return m[row];
    }
};

struct Plane {
    Vec3 normal{};
    float d{0.0f};

    float distanceToPoint(const Vec3& p) const {
        return normal.x * p.x + normal.y * p.y + normal.z * p.z + d;
    }

    void normalize() {
        const float len = std::sqrt(
            normal.x * normal.x +
            normal.y * normal.y +
            normal.z * normal.z
        );
        if (len > 1e-6f) {
            const float inv = 1.0f / len;
            normal.x *= inv;
            normal.y *= inv;
            normal.z *= inv;
            d *= inv;
        }
    }
};

struct Frustum {
    std::array<Plane, 6> planes{};

    static Frustum fromViewProjection(const Matrix4& m);
    bool intersects(const AABB& box) const;
};

struct CameraState {
    Vec3 position{};
    Matrix4 viewProjection{};
    Frustum frustum{};
    bool valid{false};
};
