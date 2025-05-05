#include "mge/behaviours/AbstractBehaviour.hpp"
#include <vector>

class CamKeysBehaviour : public AbstractBehaviour {
public:
    CamKeysBehaviour(float pMoveSpeed = 5.0f, float pTurnSpeed = 45.0f);
    virtual ~CamKeysBehaviour();
    virtual void update(float pStep);

    void setBehaviourTarget(GameObject* target);
    void setTargets(const std::vector<GameObject*>& targets);

private:
    float _moveSpeed;
    float _turnSpeed;
    GameObject* _target = nullptr;
    std::vector<GameObject*> _targets;
    int _currentTargetIndex = 0;

    bool _key1WasPressed = false;
    bool _key2WasPressed = false;
};
