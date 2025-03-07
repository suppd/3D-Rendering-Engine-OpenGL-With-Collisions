#include <iostream>
#include <string>

#include "glm.hpp"

#include "mge/core/Renderer.hpp"

#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/GameObject.hpp"

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/TestGame.hpp"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
TestGame::TestGame():AbstractGame(), _hud(0)
{
}

void TestGame::initialize() {
    //setup the core part
    AbstractGame::initialize();

    //setup the custom part so we can display some text
    std::cout << "Initializing HUD" << std::endl;
    _hud = new DebugHud(_window);
    std::cout << "HUD initialized." << std::endl << std::endl;
}

//build the game _world
void TestGame::_initializeScene()
{
    //MESHES

    //load a bunch of meshes we will be using throughout this demo
    //each mesh only has to be loaded once, but can be used multiple times:
    //F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
    Mesh* planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
    Mesh* cubeMeshF = Mesh::load(config::MGE_MODEL_PATH + "cube_flat.obj");
    Mesh* sphereMeshS = Mesh::load(config::MGE_MODEL_PATH + "sphere_smooth.obj");

    //MATERIALS

    //create some materials to display the cube, the plane and the light
    AbstractMaterial* lightMaterial = new ColorMaterial(glm::vec3(1, 1, 0));
    AbstractMaterial* runicStoneMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "runicfloor.png"));

    // Create two different color materials
    AbstractMaterial* redColorMaterial = new ColorMaterial(glm::vec3(1, 0, 0)); // Red color
    AbstractMaterial* blueColorMaterial = new ColorMaterial(glm::vec3(0, 0, 1)); // Blue color

    // Create two different texture materials
    AbstractMaterial* runicStoneMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "runicfloor.png"));
    AbstractMaterial* bricksMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"));

    // Create a movable GameObject with KeyBehaviour
    GameObject* movableStructure = new GameObject("movable", glm::vec3(0, 0, 0));
    movableStructure->setBehaviour(new KeysBehaviour(5)); // 5 is the speed or some other relevant parameter

    // Create a nested GameObject and add it to the movable structure
    GameObject* childObject = new GameObject("child", glm::vec3(1, 0, 0)); // Positioned relative to its parent
    childObject->setMesh(cubeMeshF); // Set the flat-shaded cube mesh
    childObject->setMaterial(runicStoneMaterial); // Set a texture material
    movableStructure->add(childObject); // Add the child to the movable structure

    // Add another nested GameObject
    GameObject* anotherChild = new GameObject("anotherChild", glm::vec3(-1, 0, 0));
    anotherChild->setMesh(sphereMeshS); // Set the smooth-shaded sphere mesh
    anotherChild->setMaterial(bricksMaterial); // Set another texture material
    movableStructure->add(anotherChild);

    // Add the movable structure to the world
    _world->add(movableStructure);

    // Set up the camera to follow the movable structure
    Camera* camera = _world->getMainCamera();
    glm::vec3 offset(0, 2, 5); // Offset the camera above and behind the structure
    camera->setTransform(movableStructure->getTransform() * glm::translate(offset));
    // You will need to update this transformation each frame if the structure moves

}

void TestGame::_render() {
    AbstractGame::_render();
    _updateHud();
}

void TestGame::_updateHud() {
    std::string debugInfo = "";
    debugInfo += std::string("FPS:") + std::to_string((int)_fps) + "\n";

    _hud->setDebugInfo(debugInfo);
    _hud->draw();
}

TestGame::~MGEDemo()
{
    //dtor
}
