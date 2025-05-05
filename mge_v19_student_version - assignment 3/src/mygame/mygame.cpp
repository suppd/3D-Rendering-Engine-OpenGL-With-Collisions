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
#include "mge/util/TestManager.hpp"
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
    srand(10399); //always same fixed seed
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

    Light* light = new Light("light", glm::vec3(3,6,0));
    light->scale(glm::vec3(0.1f, 0.1f, 0.1f));
    light->setMesh(cubeMeshF);
    light->setMaterial(lightMaterial);
    light->setAmbientStrength(3);
    light->setBehaviour(new KeysBehaviour(25,25));
    _world->add(light);
    _world->setMainLight(light);

    //first generate random objects
    generateRandomObjects(100);
    TestManager tester;
    //then run tests on these objects
    tester.RunAllTests(_gameObjects);

    CamKeysBehaviour* camBehaviour = new CamKeysBehaviour();
    camBehaviour->setTargets(_gameObjects);
    camera->setBehaviour(camBehaviour);

}


void mygame::_render() {
    AbstractGame::_render();
    _updateHud();

    //do debug drawing of OBB and AABB to visualize them
    for (GameObject* obj : _gameObjects) {
        // Draw OBB in blue
        OBB obb = OBB::ComputeOBBForGameObject(obj);
        DebugDraw::DrawOBB(obb, glm::vec3(0, 0, 1)); 

        // Draw AABB in green
        AABB aabb = AABB::ComputeAABBForGameObject(obj);
        DebugDraw::DrawAABB(aabb, glm::vec3(0, 1, 0)); 
        //std::cout << "OBB and AABB created for: " << obj->getName() << std::endl;
    }

    DebugDraw::Render(_world->getMainCamera()->getView(), _world->getMainCamera()->getProjection());
}


void mygame::_updateHud() {
    std::string debugInfo = "";
    debugInfo += std::string ("FPS:") + std::to_string((int)_fps)+"\n";

    _hud->setDebugInfo(debugInfo);
    _hud->draw();
}

void mygame::generateRandomObjects(int count) {
    Mesh* sphereMesh = Mesh::load(config::MGE_MODEL_PATH + "sphere_smooth.obj");
    Mesh* cubeMesh = Mesh::load(config::MGE_MODEL_PATH + "cube_flat.obj");
    Mesh* teapotMesh = Mesh::load(config::MGE_MODEL_PATH + "teapot_smooth.obj");

    AbstractMaterial* redMaterial = new ColorMaterial(glm::vec3(1, 0, 0), _world->getMainCamera());
    AbstractMaterial* greenMaterial = new ColorMaterial(glm::vec3(0, 1, 0), _world->getMainCamera());

    //random position
    for (int i = 0; i < count; ++i) {
        float x = (rand() % 400 - 200) / 5.0f; // [-40, 40]
        float y = (rand() % 200) / 10.0f;      // [0, 20]
        float z = (rand() % 400 - 200) / 5.0f; // [-40, 40]
        // scale 
         float scale = (1.0f);
        //float scale = (rand() % 100) / 100.0f + 0.5f; // [0.5,1.5]
        //choose mesh
        Mesh* mesh = cubeMesh; 
        // (i % 2 == 0) ? cubeMesh : sphereMesh;
        //Give the specific meshes uniform colors
        AbstractMaterial* mat = redMaterial; 
        //(i % 2 == 0) ? redMaterial : greenMaterial;

        GameObject* obj = new GameObject("randObj_" + std::to_string(i), glm::vec3(0));
        obj->setMesh(mesh);
        obj->setMaterial(mat);
        obj->setLocalPosition(glm::vec3(x, y, z));
        obj->scale(glm::vec3(scale));

        //random rotation
        //float angle = (rand() % 360);
        //obj->rotate(glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));

        _world->add(obj);
        _gameObjects.push_back(obj);
    }

    std::cout << count << " random objects generated.\n";
}

mygame::~mygame()
{
	//dtor
}
