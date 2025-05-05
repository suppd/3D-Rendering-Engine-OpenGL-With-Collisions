#include "DebugDraw.hpp"
#include "mge/collision/OBB.hpp"
#include <GL/glew.h>
#include <iostream>
#include "mge/config.hpp"

ShaderProgram* DebugDraw::_shader = nullptr;
std::vector<glm::vec3> DebugDraw::_vertices;
std::vector<glm::vec3> DebugDraw::_colors;
GLuint DebugDraw::_vao = 0;
GLuint DebugDraw::_vbo = 0;
GLuint DebugDraw::_cbo = 0;

void DebugDraw::Initialize() {
    _shader = new ShaderProgram();
    _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "debug.vs");
    _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "debug.fs");
    _shader->finalize();

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_cbo);

    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, _cbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void DebugDraw::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color) {
    _vertices.push_back(start);
    _vertices.push_back(end);
    _colors.push_back(color);
    _colors.push_back(color);
}

void DebugDraw::DrawOBB(const OBB& obb, const glm::vec3& color) {
    glm::vec3 corners[8];
    obb.getCorners(corners);

    DrawLine(corners[0], corners[1], color);
    DrawLine(corners[1], corners[2], color);
    DrawLine(corners[2], corners[3], color);
    DrawLine(corners[3], corners[0], color);

    DrawLine(corners[4], corners[5], color);
    DrawLine(corners[5], corners[6], color);
    DrawLine(corners[6], corners[7], color);
    DrawLine(corners[7], corners[4], color);

    DrawLine(corners[0], corners[4], color);
    DrawLine(corners[1], corners[5], color);
    DrawLine(corners[2], corners[6], color);
    DrawLine(corners[3], corners[7], color);
}
void DebugDraw::DrawAABB(const AABB& aabb, const glm::vec3& color) {
    glm::vec3 min = aabb.min;
    glm::vec3 max = aabb.max;

    // Define the 8 corners of the AABB
    glm::vec3 corners[8] = {
        glm::vec3(min.x, min.y, min.z),
        glm::vec3(max.x, min.y, min.z),
        glm::vec3(max.x, max.y, min.z),
        glm::vec3(min.x, max.y, min.z),
        glm::vec3(min.x, min.y, max.z),
        glm::vec3(max.x, min.y, max.z),
        glm::vec3(max.x, max.y, max.z),
        glm::vec3(min.x, max.y, max.z)
    };

    // Draw the 12 edges of the AABB
    DrawLine(corners[0], corners[1], color);
    DrawLine(corners[1], corners[2], color);
    DrawLine(corners[2], corners[3], color);
    DrawLine(corners[3], corners[0], color);

    DrawLine(corners[4], corners[5], color);
    DrawLine(corners[5], corners[6], color);
    DrawLine(corners[6], corners[7], color);
    DrawLine(corners[7], corners[4], color);

    DrawLine(corners[0], corners[4], color);
    DrawLine(corners[1], corners[5], color);
    DrawLine(corners[2], corners[6], color);
    DrawLine(corners[3], corners[7], color);
}

void DebugDraw::Render(const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    if (_vertices.empty()) return;

    _shader->use();

    glDisable(GL_DEPTH_TEST);

    glLineWidth(2.0f);
    // Set view and projection matrices
    glUniformMatrix4fv(_shader->getUniformLocation("viewMatrix"), 1, GL_FALSE, &pViewMatrix[0][0]);
    glUniformMatrix4fv(_shader->getUniformLocation("projectionMatrix"), 1, GL_FALSE, &pProjectionMatrix[0][0]);

    glBindVertexArray(_vao);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3), &_vertices[0], GL_STATIC_DRAW);

    // Upload color data
    glBindBuffer(GL_ARRAY_BUFFER, _cbo);
    glBufferData(GL_ARRAY_BUFFER, _colors.size() * sizeof(glm::vec3), &_colors[0], GL_STATIC_DRAW);

    glDrawArrays(GL_LINES, 0, _vertices.size());

    glBindVertexArray(0);

    _vertices.clear();
    _colors.clear();
}