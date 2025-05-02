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
    glm::vec3 min = obj->getWorldTransform() * glm::vec4(localAABB.min, 1.0f);
    glm::vec3 max = obj->getWorldTransform() * glm::vec4(localAABB.max, 1.0f);
    return AABB(min, max);
}
