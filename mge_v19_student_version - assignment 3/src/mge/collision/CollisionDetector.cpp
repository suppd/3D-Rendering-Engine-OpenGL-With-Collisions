#include "CollisionDetector.hpp"
#include <glm/glm.hpp>

CollisionDetector::CollisionDetector(GameObject* obj1, GameObject* obj2) : obj1(obj1), obj2(obj2) {}

bool CollisionDetector::checkCollisionOBB() {
    OBB obb1 = createOBBForGameObject(obj1);
    OBB obb2 = createOBBForGameObject(obj2);

    return obb1.intersects(obb2);
}


OBB CollisionDetector::createOBBForGameObject(GameObject* obj) {
    //glm::vec3 center = obj->getWorldPosition();
    //glm::vec3 extents = obj->getScale() * 0.5f; // Assuming the mesh is a unit cube
    //glm::mat3 orientation = glm::mat3(obj->getWorldTransform());

    //return OBB(center, extents, orientation);
    Mesh* tempMesh;
    tempMesh = obj->getMesh();
    std::vector<glm::vec3> vertices = tempMesh->_vertices;

    // Calculate the center (mean of vertices)
    glm::vec3 center(0.0f);
    for (const auto& vertex : vertices) {
        center += vertex;
    }
    center /= vertices.size();

    // Calculate extents (half the width, height, depth)
    glm::vec3 minVertex = vertices[0];
    glm::vec3 maxVertex = vertices[0];
    for (const auto& vertex : vertices) {
        minVertex = glm::min(minVertex, vertex);
        maxVertex = glm::max(maxVertex, vertex);
    }
    glm::vec3 extents = (maxVertex - minVertex) * 0.5f;

    // Calculate orientation (identity matrix for simplicity, can be enhanced)
    glm::mat3 orientation(1.0f);

    return OBB(center, extents, orientation);
}

bool CollisionDetector::checkCollisionAABB(const AABB& aabb1, const AABB& aabb2) {
    return (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
        (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
        (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z);
}