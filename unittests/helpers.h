#pragma once

#include <cmath>


inline bool equals(const knm::tr2::vec3& v1, const knm::tr2::vec3& v2, float tolerance = 1e-4f)
{
    return (abs(v1.x - v2.x) < tolerance) && (abs(v1.y - v2.y) < tolerance) &&
           (abs(v1.z - v2.z) < tolerance);
}


inline bool equals(const knm::tr2::quat& lhs, const knm::tr2::quat& rhs, float tolerance=1e-3f)
{
    float fCos = KNM_TRANSFORMS_TREE_DOT(lhs, rhs);
    float angle = acos(fCos);

    return (abs(angle) <= tolerance) || (abs(angle - knm::tr2::PI) < tolerance);
}
