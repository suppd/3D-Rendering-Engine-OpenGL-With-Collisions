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
#include "mge/behaviours/CamKeysBehaviour.hpp"

#include "mge/collision/CollisionDetector.hpp"
#include "mge/util/DebugDraw.hpp"
#include "mge/collision/AABB.hpp"

#include "mge/util/DebugHud.hpp"

#include "mygame/config.hpp"
#include "mygame/mygame.hpp"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
mygame::mygame():AbstractGame (),_hud(0)
{
}

void mygame::initialize() {
    //setup the core part
    AbstractGame::initialize();
    DebugDraw::Initialize();
    //setup the custom part so we can display some text
	std::cout << "Initializing HUD" << std::endl;
	_hud = new DebugHud(_window);
	std::cout << "HUD initialized." << std::endl << std::endl;
}

//build the game _world
void mygame::_initializeScene()
{

    Camera* camera = new Camera("camera", glm::vec3(0, 6, 7));
    //MESHES

    //load a bunch of meshes we will be using throughout this demo
    //each mesh only has to be loaded once, but can be used multiple times:
    //F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
    //4 Meshes
    Mesh* planeMeshDefault = Mesh::load (config::MGE_MODEL_PATH+"plane.obj");
    Mesh* cubeMeshF = Mesh::load (config::MGE_MODEL_PATH+"cube_flat.obj");
    Mesh* sphereMeshS = Mesh::load (config::MGE_MODEL_PATH+"sphere_smooth.obj");
    Mesh* teampotMeshS = Mesh::load(config::MGE_MODEL_PATH + "teapot_smooth.obj");

    //MATERIALS

    //create some materials to display the cube, the plane and the light
    AbstractMaterial* lightMaterial = new ColorMaterial (glm::vec3(1,1,0),camera);
    AbstractMaterial* runicStoneMaterial = new TextureMaterial (Texture::load (config::MGE_TEXTURE_PATH+"runicfloor.png"), camera);
    //2 Textures
    AbstractMaterial* landMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "land.jpg"),camera);
    AbstractMaterial* brickMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"), camera);
    //2 Colors
    ColorMaterial* redMaterial = new ColorMaterial(glm::vec3(1, 0, 0), camera);
    ColorMaterial* blueMaterial = new ColorMaterial(glm::vec3(0, 1, 0), camera);

    // Assuming you have a SkyboxMaterial and SkyboxShader
    AbstractMaterial* skyboxMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "skybox.png"), camera);




    //SCENE SETUP

   //add camera first (it will be updated last)
  
    camera->rotate(glm::radians(0.0f), glm::vec3(1,0,0));
    camera->setLocalPosition(glm::vec3(0, 5,5));
    _world->add(camera);
    _world->setMainCamera(camera);


    //non nested objects
    GameObject* environmentSphere = new GameObject("EnSphere", glm::vec3(0, 0, 0));
    environmentSphere->scale(glm::vec3(1, 1, 1));
    environmentSphere->setLocalPosition(glm::vec3(1, 0, 0));
    environmentSphere->setMesh(sphereMeshS);
    environmentSphere->setMaterial(brickMaterial);
    _world->add(environmentSphere);
    _gameObjects.push_back(environmentSphere);

    //add the plane
    GameObject* plane = new GameObject ("plane", glm::vec3(0,0,0));
    plane->scale(glm::vec3(1,1,1));
    plane->setMesh(planeMeshDefault);
    plane->setMaterial(runicStoneMaterial);
    plane->setBehaviour(new KeysBehaviour());
    _world->add(plane);
    _gameObjects.push_back(plane);
    camera->setParent(plane);

    //add a sphere
    //GameObject* sphere = new GameObject ("sphere", glm::vec3(0,0,0));
    //sphere->scale(glm::vec3(0.4,0.4,0.4));
    //sphere->setLocalPosition(glm::vec3(1, 0, 1));
    //sphere->setMesh (sphereMeshS);
    //sphere->setMaterial(runicStoneMaterial);
    ////sphere->setParent(plane);
    //_world->add(sphere);
    //_gameObjects.push_back(sphere);

    GameObject* sphere = new GameObject("sphere", glm::vec3(0, 0, 0));
    sphere->setMesh(sphereMeshS);
    sphere->setLocalPosition(glm::vec3(0, 2, 0));
    sphere->scale(glm::vec3(0.4f, 0.4f, 0.4f)); // No scaling
    sphere->setMaterial(runicStoneMaterial);
    _world->add(sphere);
    _gameObjects.push_back(sphere);

    //add a teapot 
    GameObject* teapot = new GameObject("teapot", glm::vec3(0, 0, 0));
    teapot->scale(glm::vec3(0.4, 0.4, 0.4));
    teapot->setLocalPosition(glm::vec3(0, 2, 0));
    teapot->setMesh(teampotMeshS);
    teapot->setBehaviour(new RotatingBehaviour());
    teapot->setMaterial(redMaterial);
    //teapot->setParent(plane);
    _world->add(teapot);
    _gameObjects.push_back(teapot);

    //plane->add(teapot);
    plane->add(sphere);

    camera->setBehaviour(new CamKeysBehaviour());
    camera->setBehaviourTarget(plane, camera->getBehaviour());


    Light* light = new Light("light", glm::vec3(3,6,0));
    light->scale(glm::vec3(0.1f, 0.1f, 0.1f));
    light->setMesh(cubeMeshF);
    light->setMaterial(lightMaterial);
    light->setBehaviour(new KeysBehaviour(25));
    _world->add(light);
    _world->setMainLight(light);

}


void mygame::_render() {
    AbstractGame::_render();
    _updateHud();

    for (GameObject* obj : _gameObjects) {
        // Draw OBB in red
        OBB obb = createOBBForGameObject(obj);
        DebugDraw::DrawOBB(obb, glm::vec3(1, 0, 0)); 

        // Draw AABB in green
        AABB aabb = computeAABBForGameObject(obj);
        DebugDraw::DrawAABB(aabb, glm::vec3(0, 1, 0)); 
        std::cout << "OBB and AABB created for: " << obj->getName() << std::endl;
    }
    //draw the collision boxes using debug shader
    DebugDraw::Render(_world->getMainCamera()->getView(), _world->getMainCamera()->getProjection());
}

OBB mygame::createOBBForGameObject(GameObject* obj) {
    glm::vec3 center = obj->getWorldPosition();

    // Compute local AABB
    AABB localAABB = AABB::computeLocalAABB(obj->getMesh());
    glm::vec3 localExtents = (localAABB.max - localAABB.min) * 0.5f;

    // Extract world transform (rotation + scale)
    glm::mat3 worldTransform = glm::mat3(obj->getWorldTransform());

    // Transform the extents using absolute rotation
    glm::vec3 extents = glm::vec3(
        glm::length(worldTransform[0] * localExtents),
        glm::length(worldTransform[1] * localExtents),
        glm::length(worldTransform[2] * localExtents)
    );

    glm::mat3 orientation = worldTransform;

    return OBB(center, extents, orientation);
}

AABB mygame::computeAABBForGameObject(GameObject* obj) {
    // Compute local AABB
    AABB localAABB = AABB::computeLocalAABB(obj->getMesh());

    // Transform the AABB to world space
    glm::vec3 min = obj->getWorldTransform() * glm::vec4(localAABB.min, 1.0f);
    glm::vec3 max = obj->getWorldTransform() * glm::vec4(localAABB.max, 1.0f);

    return AABB(min, max);
}

void mygame::_updateHud() {
    std::string debugInfo = "";
    debugInfo += std::string ("FPS:") + std::to_string((int)_fps)+"\n";

    _hud->setDebugInfo(debugInfo);
    _hud->draw();
}

void mygame::_checkCollisions() {
    for (size_t i = 0; i < _gameObjects.size(); ++i) {
        for (size_t j = i + 1; j < _gameObjects.size(); ++j) {
            // Check OBB collisions
            OBB obb1 = createOBBForGameObject(_gameObjects[i]);
            OBB obb2 = createOBBForGameObject(_gameObjects[j]);
            DebugDraw::DrawOBB(obb1, glm::vec3(1, 0, 0)); // Red color for OBB
            DebugDraw::DrawOBB(obb2, glm::vec3(1, 0, 0)); // Red color for OBB

            if (obb1.intersects(obb2)) {
                std::cout << "OBB Collision detected between " << _gameObjects[i]->getName()
                    << " and " << _gameObjects[j]->getName() << std::endl;
            }

            // Check AABB collisions
            AABB aabb1 = computeAABBForGameObject(_gameObjects[i]);
            AABB aabb2 = computeAABBForGameObject(_gameObjects[j]);
            DebugDraw::DrawAABB(aabb1, glm::vec3(0, 1, 0)); // Green color for AABB
            DebugDraw::DrawAABB(aabb2, glm::vec3(0, 1, 0)); // Green color for AABB

            if (CollisionDetector::checkCollisionAABB(aabb1, aabb2)) {
                std::cout << "AABB Collision detected between " << _gameObjects[i]->getName()
                    << " and " << _gameObjects[j]->getName() << std::endl;
            }
        }
    }
}


mygame::~mygame()
{
	//dtor
}
