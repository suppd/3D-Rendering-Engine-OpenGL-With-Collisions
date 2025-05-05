#include "AbstractBehaviour.hpp"

AbstractBehaviour::AbstractBehaviour():_owner(nullptr), _target(nullptr) {}

AbstractBehaviour::~AbstractBehaviour()
{
	_owner = nullptr;
    _target = nullptr;
}

void AbstractBehaviour::setOwner (GameObject* pOwner) {
    _owner = pOwner;
}


void AbstractBehaviour::setTarget(GameObject* pTarget) {
    _target = pTarget;
}