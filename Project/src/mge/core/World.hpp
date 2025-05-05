#ifndef WORLD_HPP
#define WORLD_HPP

#include "mge/core/GameObject.hpp"

class Camera;
class Light;

class World : public GameObject
{
	public:
        World();

		void setMainCamera (Camera* pCamera);
		Camera* getMainCamera();

        void setMainLight(Light* pLight);
        Light* getMainLight();

        //only used internally, do not use from outside
        void registerLight (Light* pLight);
        void unregisterLight (Light* pLight);

        Light* getLightAt (int pIndex);
        int getLightCount();

	private:
	    Camera* _mainCamera;
	    std::vector<Light*> _lights;

        Light* _mainLight;

        World(const World&);
        World& operator=(const World&);
};


#endif // WORLD_HPP
