#pragma once

#include <vector>
#include <string>
#include "mge/core/GameObject.hpp"
#include "mge/collision/AABB.hpp"
#include "mge/collision/OBB.hpp"
#include "mge/collision/CollisionDetector.hpp"
#include "mge/util/DebugDraw.hpp"

class TestManager {
public:
    static void RunAllTests(const std::vector<GameObject*>& objects);
private:
    static void RunAccuracyTest(const std::vector<GameObject*>& objects);
    static void RunPerformanceTest(const std::vector<GameObject*>& objects);
    static void RunConsistencyTest(const std::vector<GameObject*>& objects);
    static void RunScalingTest(const std::vector<GameObject*>& objects);
};

