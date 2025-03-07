#ifndef OBB_HPP
#define OBB_HPP

#include <glm/glm.hpp>

struct OBB {
    glm::vec3 center;
    glm::vec3 extents;
    glm::mat3 orientation;

    OBB() : center(glm::vec3(0)), extents(glm::vec3(0)), orientation(glm::mat3(1)) {}
    OBB(const glm::vec3& center, const glm::vec3& extents, const glm::mat3& orientation)
        : center(center), extents(extents), orientation(orientation) {}

    bool intersects(const OBB& other) const;
    void getCorners(glm::vec3 corners[8]) const {
        glm::vec3 extentsX = orientation[0] * extents.x;
        glm::vec3 extentsY = orientation[1] * extents.y;
        glm::vec3 extentsZ = orientation[2] * extents.z;

        corners[0] = center - extentsX - extentsY - extentsZ;
        corners[1] = center + extentsX - extentsY - extentsZ;
        corners[2] = center + extentsX + extentsY - extentsZ;
        corners[3] = center - extentsX + extentsY - extentsZ;
        corners[4] = center - extentsX - extentsY + extentsZ;
        corners[5] = center + extentsX - extentsY + extentsZ;
        corners[6] = center + extentsX + extentsY + extentsZ;
        corners[7] = center - extentsX + extentsY + extentsZ;
    }
};;

#endif