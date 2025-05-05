#include "mge/collision/AABB.hpp"

bool AABB::Intersects(const AABB& other) const {
    return (min.x <= other.max.x && max.x >= other.min.x) &&
        (min.y <= other.max.y && max.y >= other.min.y) &&
        (min.z <= other.max.z && max.z >= other.min.z);
}

AABB AABB::ComputeLocalAABB(const Mesh* mesh) {
    if (!mesh || mesh->_vertices.empty()) {
        return AABB{ glm::vec3(0.0f), glm::vec3(0.0f) };
    }

    glm::vec3 min(FLT_MAX);
    glm::vec3 max(-FLT_MAX);

    for (const glm::vec3& vertex : mesh->_vertices) {
        min = glm::min(min, vertex);
        max = glm::max(max, vertex);
    }

    return AABB{ min, max };
}
AABB AABB::ComputeAABBForGameObject(GameObject* obj)
{
    AABB localAABB = AABB::ComputeLocalAABB(obj->getMesh());

    // Get all 8 corners of the local AABB
    glm::vec3 corners[8];
    corners[0] = glm::vec3(localAABB.min.x, localAABB.min.y, localAABB.min.z);
    corners[1] = glm::vec3(localAABB.max.x, localAABB.min.y, localAABB.min.z);
    corners[2] = glm::vec3(localAABB.min.x, localAABB.max.y, localAABB.min.z);
    corners[3] = glm::vec3(localAABB.max.x, localAABB.max.y, localAABB.min.z);
    corners[4] = glm::vec3(localAABB.min.x, localAABB.min.y, localAABB.max.z);
    corners[5] = glm::vec3(localAABB.max.x, localAABB.min.y, localAABB.max.z);
    corners[6] = glm::vec3(localAABB.min.x, localAABB.max.y, localAABB.max.z);
    corners[7] = glm::vec3(localAABB.max.x, localAABB.max.y, localAABB.max.z);

    glm::mat4 transform = obj->getWorldTransform();

    glm::vec3 worldMin(FLT_MAX);
    glm::vec3 worldMax(-FLT_MAX);

    for (int i = 0; i < 8; ++i) {
        glm::vec3 worldCorner = glm::vec3(transform * glm::vec4(corners[i], 1.0f));
        worldMin = glm::min(worldMin, worldCorner);
        worldMax = glm::max(worldMax, worldCorner);
    }
    return AABB(worldMin, worldMax);
}
