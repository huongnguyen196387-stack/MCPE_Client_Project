#include "client/Types.h"

Frustum Frustum::fromViewProjection(const Matrix4& m) {
    Frustum f{};

    // Row-major extraction:
    // left   = row4 + row1
    // right  = row4 - row1
    // bottom = row4 + row2
    // top    = row4 - row2
    // near   = row4 + row3
    // far    = row4 - row3
    auto setPlane = [](Plane& p, float a, float b, float c, float d) {
        p.normal = {a, b, c};
        p.d = d;
        p.normalize();
    };

    setPlane(f.planes[0],
             m[3][0] + m[0][0],
             m[3][1] + m[0][1],
             m[3][2] + m[0][2],
             m[3][3] + m[0][3]);

    setPlane(f.planes[1],
             m[3][0] - m[0][0],
             m[3][1] - m[0][1],
             m[3][2] - m[0][2],
             m[3][3] - m[0][3]);

    setPlane(f.planes[2],
             m[3][0] + m[1][0],
             m[3][1] + m[1][1],
             m[3][2] + m[1][2],
             m[3][3] + m[1][3]);

    setPlane(f.planes[3],
             m[3][0] - m[1][0],
             m[3][1] - m[1][1],
             m[3][2] - m[1][2],
             m[3][3] - m[1][3]);

    setPlane(f.planes[4],
             m[3][0] + m[2][0],
             m[3][1] + m[2][1],
             m[3][2] + m[2][2],
             m[3][3] + m[2][3]);

    setPlane(f.planes[5],
             m[3][0] - m[2][0],
             m[3][1] - m[2][1],
             m[3][2] - m[2][2],
             m[3][3] - m[2][3]);

    return f;
}

bool Frustum::intersects(const AABB& box) const {
    for (const auto& plane : planes) {
        Vec3 positive = box.min;

        if (plane.normal.x >= 0.0f) positive.x = box.max.x;
        if (plane.normal.y >= 0.0f) positive.y = box.max.y;
        if (plane.normal.z >= 0.0f) positive.z = box.max.z;

        if (plane.distanceToPoint(positive) < 0.0f) {
            return false;
        }
    }
    return true;
}
