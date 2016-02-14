#ifndef AABB_H_
#define AABB_H_

#include "Vector.h"
#include "mclib.h"

#undef min
#undef max

struct AABB {
    Vector3d min;
    Vector3d max;

    MCLIB_API AABB() : min(0, 0, 0), max(0, 0, 0) { }
    MCLIB_API AABB(const Vector3d& min, const Vector3d& max) : min(min), max(max) { }

    bool MCLIB_API Contains(Vector3d point) const {
        return (point.x >= min.x && point.x <= max.x) &&
               (point.y >= min.y && point.y <= max.y) &&
               (point.z >= min.z && point.z <= max.z);
    }

    bool MCLIB_API Intersects(const AABB& other) const {
        return !(max.x <= other.min.x || max.y <= other.min.y || max.z <= other.min.z ||
               min.x >= other.max.x || min.y >= other.max.y || min.z >= other.max.z);
    }
};

#endif