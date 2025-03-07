#ifndef DEBUGDRAW_HPP
#define DEBUGDRAW_HPP

#include <vector>
#include <glm/glm.hpp>
#include "mge/core/ShaderProgram.hpp"
#include "mge/collision/OBB.hpp"
#include "mge/collision/AABB.hpp"

class DebugDraw {
public:
    static void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color);
    static void DrawOBB(const OBB& obb, const glm::vec3& color);
    static void DrawAABB(const AABB& aabb, const glm::vec3& color);
    static void Initialize();
    static void Render(const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix);

private:
    static ShaderProgram* _shader;
    static std::vector<glm::vec3> _vertices;
    static std::vector<glm::vec3> _colors;
    static GLuint _vao, _vbo, _cbo;
};

#endif