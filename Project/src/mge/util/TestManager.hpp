#pragma once
#include <fstream>
#include <vector>
#include <string>
#include "mge/core/GameObject.hpp"
#include "mge/collision/AABB.hpp"
#include "mge/collision/OBB.hpp"
#include "mge/collision/CollisionDetector.hpp"
#include "mge/util/DebugDraw.hpp"
#include <sstream>  
#include <fstream>
#include <chrono>
#include <iostream>
#include <random>

class TestManager {
public:
    static void RunAllTests(const std::vector<GameObject*>& objects);
    static void RunAccuracyTest(const std::vector<GameObject*>& objects);
    static void RunPerformanceTest(const std::vector<GameObject*>& objects);
    static void RunConsistencyTest(const std::vector<GameObject*>& objects);
    static void RunMemoryTest(const std::vector<GameObject*>& objects);


private:
    static std::ofstream testResults;
    static void OpenFile(const std::string& filename);
    static void TestManager::WriteLine(const std::string& line);
    static void CloseFile();
};