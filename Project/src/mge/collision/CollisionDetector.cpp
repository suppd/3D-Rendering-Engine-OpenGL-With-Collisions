#include "CollisionDetector.hpp"
#include <glm/glm.hpp>

CollisionDetector::CollisionDetector(GameObject* obj1, GameObject* obj2) : obj1(obj1), obj2(obj2) {}

bool CollisionDetector::checkCollisionOBB() {
    OBB obb1 = CollisionDetector::CreateOBBForGameObject(obj1);
    OBB obb2 = CollisionDetector::CreateOBBForGameObject(obj2);

    return obb1.intersects(obb2);
}


OBB CollisionDetector::CreateOBBForGameObject(GameObject* obj) {
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

bool CollisionDetector::checkCollisionAABB(const AABB& aabb1, const AABB& aabb2) {
    return (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
        (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
        (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z);
}