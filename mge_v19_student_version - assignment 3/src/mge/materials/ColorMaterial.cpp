#include "glm.hpp"

#include "ColorMaterial.hpp"
#include "mge/config.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Camera.hpp"
ShaderProgram* ColorMaterial::_shader = NULL;

GLint __uLightPos;
GLint __uLightColor;
GLint __uAmbientStrength;


ColorMaterial::ColorMaterial(glm::vec3 pDiffuseColor, Camera* pCam)
    : _diffuseColor(pDiffuseColor),viewPos(pCam)
{
    //every time we create an instance of colormaterial we check if the corresponding shader has already been loaded
    _lazyInitializeShader();
}

void ColorMaterial::_lazyInitializeShader() {
    //this shader contains everything the material can do (it can render something in 3d using a single color)
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH+"color.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH+"color.fs");

        __uLightPos = _shader->getUniformLocation("lightPos");
        __uLightColor = _shader->getUniformLocation("lightColor");
        __uAmbientStrength = _shader->getUniformLocation("ambientStrength");




    }
        _shader->finalize();
}


ColorMaterial::~ColorMaterial()
{
    //dtor
}

void ColorMaterial::setDiffuseColor(glm::vec3 pDiffuseColor) {
    _diffuseColor = pDiffuseColor;
}

void ColorMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    _shader->use();

    //set the material color
    glUniform3fv (_shader->getUniformLocation("diffuseColor"), 1, glm::value_ptr(_diffuseColor));


    glUniform3fv(_shader->getUniformLocation("viewPos"), 1, glm::value_ptr(viewPos->getWorldPosition()));

    //pass in all MVP matrices separately
    glUniformMatrix4fv ( _shader->getUniformLocation("projectionMatrix"),   1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
    glUniformMatrix4fv ( _shader->getUniformLocation("viewMatrix"),         1, GL_FALSE, glm::value_ptr(pViewMatrix));
    glUniformMatrix4fv ( _shader->getUniformLocation("modelMatrix"),        1, GL_FALSE, glm::value_ptr(pModelMatrix));


    Light* mainLight = pWorld->getMainLight();
    if (mainLight) {
        glUniform3fv(_shader->getUniformLocation("lightPos"), 1, glm::value_ptr(mainLight->getLocalPosition()));
        glUniform3fv(_shader->getUniformLocation("lightColor"), 1, glm::value_ptr(mainLight->getColor()));
        glUniform1f(_shader->getUniformLocation("ambientStrength"), mainLight->getAmbientStrength());
    }


    //now inform mesh of where to stream its data
    pMesh->streamToOpenGL(
        _shader->getAttribLocation("vertex"),
        _shader->getAttribLocation("normal"),
        _shader->getAttribLocation("uv")
    );

}
