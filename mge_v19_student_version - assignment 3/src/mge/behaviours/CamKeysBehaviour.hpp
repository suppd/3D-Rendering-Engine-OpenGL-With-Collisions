#ifndef CAMKEYSBEHAVIOUR_HPP
#define CAMKEYSBEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"

/**
 * KeysBehaviour allows you to move an object using the keyboard in its own local space.
 * Left right turns, forward back moves.
 */
class CamKeysBehaviour : public AbstractBehaviour
{
public:
	//move speed is in units per second, turnspeed in degrees per second
	CamKeysBehaviour(float pMoveSpeed = 5, float pTurnSpeed = 45);
	virtual ~CamKeysBehaviour();
	virtual void update(float pStep);
	float angleStorage = 0.0f;
	float turnSpeed = 0.0f;

private:
	float _moveSpeed;
	float _turnSpeed;
};

#endif // KEYSBEHAVIOUR_HPP
#pragma once
