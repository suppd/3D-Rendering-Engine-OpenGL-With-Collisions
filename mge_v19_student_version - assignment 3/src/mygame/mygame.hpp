#ifndef MYGAME_HPP
#define MYGAME_HPP

#include "mge/core/AbstractGame.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/collision/OBB.hpp"
#include "mge/collision/AABB.hpp"

class DebugHud;

/**
 * An example subclass of AbstractGame showing how we can setup a scene.
 */
class mygame: public AbstractGame
{
	public:
		mygame();
		virtual ~mygame();

        //override initialize so we can add a DebugHud
        virtual void initialize();
		//OBB createOBBForGameObject(GameObject* obj);
		void mygame::generateRandomObjects(int count);
	protected:
	    //override so we can construct the actual scene
        virtual void _initializeScene();

	    //override render to render the hud as well.
	    virtual void _render();

	private:
		DebugHud* _hud;                   //hud display
		void _checkCollisions();
        void _updateHud();

        mygame(const mygame&);
		mygame& operator=(const mygame&);

		std::vector<GameObject*> _gameObjects; // Container for GameObjects
};

#endif // MGEDEMO_HPP
