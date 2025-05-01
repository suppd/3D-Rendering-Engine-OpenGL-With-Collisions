#include "mge/behaviours/CamKeysBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "mge/core/Renderer.hpp"
#include <fstream>
#include <iostream>

CamKeysBehaviour::CamKeysBehaviour(float pMoveSpeed, float pTurnSpeed) : AbstractBehaviour(), _moveSpeed(pMoveSpeed), _turnSpeed(pTurnSpeed)
{
}

CamKeysBehaviour::~CamKeysBehaviour()
{
}

void CamKeysBehaviour::update(float pStep)
{
    float moveSpeed = 0.0f; // Default if no keys
    float angleY = 0.0f;
    float angleX = 0.0f;
    float angleZ = 0.0f; // For Z-axis rotation

    // Check if the left mouse button is pressed
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        // Rotate around Y and X axes
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            angleX += _turnSpeed * pStep;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            angleX -= _turnSpeed * pStep;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            angleY += _turnSpeed * pStep;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            angleY -= _turnSpeed * pStep;
        }
        // Rotate around Z-axis
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            angleZ += _turnSpeed * pStep;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            angleZ -= _turnSpeed * pStep;
        }

        // Apply the rotations
        _owner->rotate(glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
        _owner->rotate(glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
        _owner->rotate(glm::radians(angleZ), glm::vec3(0.0f, 0.0f, 1.0f));

        // Set the camera's position to the target's position
        _owner->setLocalPosition(_target->getLocalPosition());

        // Translate the camera to maintain a fixed distance from the target
        _owner->translate(glm::vec3(0.0f, 0.0f, 5.0f));
    }

    // Movement in all directions
    // Vertical movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        _owner->translate(glm::vec3(0.0f, _moveSpeed * pStep, 0.0f)); // Move up
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
        _owner->translate(glm::vec3(0.0f, -_moveSpeed * pStep, 0.0f)); // Move down
    }
    // Horizontal and forward/backward movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        _owner->translate(glm::vec3(-_moveSpeed * pStep, 0.0f, 0.0f)); // Move left
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        _owner->translate(glm::vec3(_moveSpeed * pStep, 0.0f, 0.0f)); // Move right
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        _owner->translate(glm::vec3(0.0f, 0.0f, _moveSpeed * pStep)); // Move forward
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        _owner->translate(glm::vec3(0.0f, 0.0f, -_moveSpeed * pStep)); // Move backward
    }
}
