#ifndef COLLISIONDETECTOR_HPP
#define COLLISIONDETECTOR_HPP

#include "mge/core/Mesh.hpp"
#include "mge/collision/OBB.hpp"
#include <vector>
#include "mge/core/GameObject.hpp"
#include "AABB.hpp"

class CollisionDetector {
public:
    CollisionDetector(GameObject* obj1, GameObject* obj2);
    bool checkCollisionOBB();
    OBB CreateOBBForGameObject(GameObject* obj);
    static bool checkCollisionAABB(const AABB& aabb1, const AABB& aabb2);
private:
    GameObject* obj1;
    GameObject* obj2;

    
};

#endif 