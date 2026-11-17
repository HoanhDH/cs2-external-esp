#pragma once

#include <algorithm>
#include <cmath>

// Standalone geometry raycast for test scenes with known collision boxes.
// Does not read CS2 maps, engine physics, or game process memory.
namespace demo_raycast {
struct Point3 {
    float x, y, z;
};

struct Box3 {
    Point3 min;
    Point3 max;
};

// The slab algorithm clips a finite segment to an axis-aligned box.
// Returns true if a solid obstruction intersects any point on the segment.
inline bool SegmentHitsBox(const Point3& from, const Point3& to, const Box3& box) {
    float min_t = 0.0f;
    float max_t = 1.0f;

    const float origin[] = { from.x, from.y, from.z };
    const float delta[] = { to.x - from.x, to.y - from.y, to.z - from.z };
    const float lo[] = { box.min.x, box.min.y, box.min.z };
    const float hi[] = { box.max.x, box.max.y, box.max.z };

    for (int axis = 0; axis < 3; ++axis) {
        if (std::abs(delta[axis]) < 1e-7f) {
            if (origin[axis] < lo[axis] || origin[axis] > hi[axis])
                return false;
            continue;
        }

        const float inv = 1.0f / delta[axis];
        const float t1 = (lo[axis] - origin[axis]) * inv;
        const float t2 = (hi[axis] - origin[axis]) * inv;
        min_t = std::max(min_t, std::min(t1, t2));
        max_t = std::min(max_t, std::max(t1, t2));

        if (min_t > max_t)
            return false;
    }
    return true;
}

inline bool ClearLineOfSight(const Point3& from, const Point3& target, const Box3& obstacle) {
    return !SegmentHitsBox(from, target, obstacle);
}
} // namespace demo_raycast
