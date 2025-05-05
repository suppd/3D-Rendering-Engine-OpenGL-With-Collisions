#include "OBB.hpp"
#include <glm/glm.hpp>
#include <algorithm>
#include <cmath>

#include "OBB.hpp"
#include <glm/glm.hpp>
#include <algorithm>
#include <cmath>

bool OBB::intersects(const OBB& other) const {
    glm::mat3 R;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            R[i][j] = glm::dot(orientation[i], other.orientation[j]);

    glm::vec3 t = other.center - center;
    t = glm::vec3(glm::dot(t, orientation[0]), glm::dot(t, orientation[1]), glm::dot(t, orientation[2]));

    glm::mat3 AbsR;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            AbsR[i][j] = std::abs(R[i][j]) + 0.0001f;


    for (int i = 0; i < 3; i++) {
        float ra = extents[i];
        float rb = other.extents[0] * AbsR[i][0] + other.extents[1] * AbsR[i][1] + other.extents[2] * AbsR[i][2];
        if (std::abs(t[i]) > ra + rb) return false;
    }


    for (int i = 0; i < 3; i++) {
        float ra = extents[0] * AbsR[0][i] + extents[1] * AbsR[1][i] + extents[2] * AbsR[2][i];
        float rb = other.extents[i];
        if (std::abs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb) return false;
    }


    float ra = extents[1] * AbsR[2][0] + extents[2] * AbsR[1][0];
    float rb = other.extents[1] * AbsR[0][2] + other.extents[2] * AbsR[0][1];
    if (std::abs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) return false;


    ra = extents[1] * AbsR[2][1] + extents[2] * AbsR[1][1];
    rb = other.extents[0] * AbsR[0][2] + other.extents[2] * AbsR[0][0];
    if (std::abs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb) return false;


    ra = extents[1] * AbsR[2][2] + extents[2] * AbsR[1][2];
    rb = other.extents[0] * AbsR[0][1] + other.extents[1] * AbsR[0][0];
    if (std::abs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb) return false;


    ra = extents[0] * AbsR[2][0] + extents[2] * AbsR[0][0];
    rb = other.extents[1] * AbsR[1][2] + other.extents[2] * AbsR[1][1];
    if (std::abs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb) return false;


    ra = extents[0] * AbsR[2][1] + extents[2] * AbsR[0][1];
    rb = other.extents[0] * AbsR[1][2] + other.extents[2] * AbsR[1][0];
    if (std::abs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb) return false;


    ra = extents[0] * AbsR[2][2] + extents[2] * AbsR[0][2];
    rb = other.extents[0] * AbsR[1][1] + other.extents[1] * AbsR[1][0];
    if (std::abs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb) return false;


    ra = extents[0] * AbsR[1][0] + extents[1] * AbsR[0][0];
    rb = other.extents[1] * AbsR[2][2] + other.extents[2] * AbsR[2][1];
    if (std::abs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb) return false;


    ra = extents[0] * AbsR[1][1] + extents[1] * AbsR[0][1];
    rb = other.extents[0] * AbsR[2][2] + other.extents[2] * AbsR[2][0];
    if (std::abs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb) return false;


    ra = extents[0] * AbsR[1][2] + extents[1] * AbsR[0][2];
    rb = other.extents[0] * AbsR[2][1] + other.extents[1] * AbsR[2][0];
    if (std::abs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) return false;

    return true;
}

float OBB::getVolume() {
    return 2.0f * extents.x * 2.0f * extents.y * 2.0f * extents.z;

}

OBB OBB::ComputeOBBForGameObject(GameObject* obj) {
    glm::vec3 center = obj->getWorldPosition();

    AABB localAABB = AABB::ComputeLocalAABB(obj->getMesh());
    glm::vec3 localCenter = (localAABB.min + localAABB.max) * 0.5f;
    glm::vec3 localExtents = (localAABB.max - localAABB.min) * 0.5f;

    glm::mat4 world = obj->getWorldTransform();
    glm::mat3 rotationScale = glm::mat3(world);

    // Transform local center into world space
    glm::vec3 worldCenter = glm::vec3(world * glm::vec4(localCenter, 1.0f));

    // Each column of the rotationScale matrix represents a local axis in world space, scaled
    glm::vec3 axisX = rotationScale[0] * localExtents.x;
    glm::vec3 axisY = rotationScale[1] * localExtents.y;
    glm::vec3 axisZ = rotationScale[2] * localExtents.z;

    glm::vec3 worldExtents = glm::vec3(glm::length(axisX), glm::length(axisY), glm::length(axisZ));

    glm::mat3 orientation = glm::mat3(glm::normalize(rotationScale[0]),
        glm::normalize(rotationScale[1]),
        glm::normalize(rotationScale[2]));

    return OBB(worldCenter, worldExtents, orientation);
}
