#include "OBB.hpp"
#include <glm/glm.hpp>
#include <algorithm>
#include <cmath>

bool OBB::intersects(const OBB& other) const {
    // Calculate the rotation matrix expressing `other` in `this`'s coordinate frame
    glm::mat3 R;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            R[i][j] = glm::dot(orientation[i], other.orientation[j]);

    // Compute the translation vector `t`
    glm::vec3 t = other.center - center;
    t = glm::vec3(glm::dot(t, orientation[0]), glm::dot(t, orientation[1]), glm::dot(t, orientation[2]));

    // Compute common subexpressions
    glm::mat3 AbsR;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            AbsR[i][j] = std::abs(R[i][j]) + 0.0001f; // Add a small value to prevent division by zero

    // Test axes L = A0, A1, A2
    for (int i = 0; i < 3; i++) {
        float ra = extents[i];
        float rb = other.extents[0] * AbsR[i][0] + other.extents[1] * AbsR[i][1] + other.extents[2] * AbsR[i][2];
        if (std::abs(t[i]) > ra + rb) return false;
    }

    // Test axes L = B0, B1, B2
    for (int i = 0; i < 3; i++) {
        float ra = extents[0] * AbsR[0][i] + extents[1] * AbsR[1][i] + extents[2] * AbsR[2][i];
        float rb = other.extents[i];
        if (std::abs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb) return false;
    }

    // Test axis L = A0 x B0
    float ra = extents[1] * AbsR[2][0] + extents[2] * AbsR[1][0];
    float rb = other.extents[1] * AbsR[0][2] + other.extents[2] * AbsR[0][1];
    if (std::abs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) return false;

    // Test axis L = A0 x B1
    ra = extents[1] * AbsR[2][1] + extents[2] * AbsR[1][1];
    rb = other.extents[0] * AbsR[0][2] + other.extents[2] * AbsR[0][0];
    if (std::abs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb) return false;

    // Test axis L = A0 x B2
    ra = extents[1] * AbsR[2][2] + extents[2] * AbsR[1][2];
    rb = other.extents[0] * AbsR[0][1] + other.extents[1] * AbsR[0][0];
    if (std::abs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb) return false;

    // Test axis L = A1 x B0
    ra = extents[0] * AbsR[2][0] + extents[2] * AbsR[0][0];
    rb = other.extents[1] * AbsR[1][2] + other.extents[2] * AbsR[1][1];
    if (std::abs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb) return false;

    // Test axis L = A1 x B1
    ra = extents[0] * AbsR[2][1] + extents[2] * AbsR[0][1];
    rb = other.extents[0] * AbsR[1][2] + other.extents[2] * AbsR[1][0];
    if (std::abs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb) return false;

    // Test axis L = A1 x B2
    ra = extents[0] * AbsR[2][2] + extents[2] * AbsR[0][2];
    rb = other.extents[0] * AbsR[1][1] + other.extents[1] * AbsR[1][0];
    if (std::abs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb) return false;

    // Test axis L = A2 x B0
    ra = extents[0] * AbsR[1][0] + extents[1] * AbsR[0][0];
    rb = other.extents[1] * AbsR[2][2] + other.extents[2] * AbsR[2][1];
    if (std::abs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb) return false;

    // Test axis L = A2 x B1
    ra = extents[0] * AbsR[1][1] + extents[1] * AbsR[0][1];
    rb = other.extents[0] * AbsR[2][2] + other.extents[2] * AbsR[2][0];
    if (std::abs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb) return false;

    // Test axis L = A2 x B2
    ra = extents[0] * AbsR[1][2] + extents[1] * AbsR[0][2];
    rb = other.extents[0] * AbsR[2][1] + other.extents[1] * AbsR[2][0];
    if (std::abs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) return false;

    // No separating axis found, the OBBs intersect
    return true;
}

