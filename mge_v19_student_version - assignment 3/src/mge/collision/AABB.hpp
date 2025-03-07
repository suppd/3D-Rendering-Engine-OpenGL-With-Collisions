#ifndef AABB_HPP
#define AABB_HPP

#include "glm/glm.hpp"
#include "mge/core/Mesh.hpp"

struct AABB {
public :
    glm::vec3 min;
    glm::vec3 max;

    AABB() : min(glm::vec3(0)), max(glm::vec3(0)) {}
    AABB(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}

    bool intersects(const AABB& other) const;
    static AABB computeLocalAABB(const Mesh* mesh);
};

#endif