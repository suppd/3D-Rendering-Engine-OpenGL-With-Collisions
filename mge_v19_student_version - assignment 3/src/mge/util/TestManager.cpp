#include "mge/util/TestManager.hpp"


std::ofstream TestManager::testResults;

void TestManager::RunAllTests(const std::vector<GameObject*>& objects) {
    TestManager tester;
    tester.OpenFile("Test Results.txt");
    RunAccuracyTest(objects);
    RunPerformanceTest(objects);
    RunConsistencyTest(objects);
    RunMemoryTest(objects);
    tester.CloseFile();
}

// ----------------------------
// ACCURACY = how "tight" the fit is around the target mesh
// ----------------------------
void TestManager::RunAccuracyTest(const std::vector<GameObject*>& objects) {
    std::cout << "\n--- ACCURACY TEST ---\n";

    for (GameObject* obj : objects) {
        Mesh* mesh = obj->getMesh();
        if (!obj || !mesh || mesh->_vertices.empty()) {
            std::cerr << "Invalid object or mesh for accuracy test: " << (obj ? obj->getName() : "nullptr") << std::endl;
            continue;
        }

        glm::vec3 minV(FLT_MAX), maxV(-FLT_MAX);
        for (const glm::vec3& v : mesh->_vertices) {
            minV = glm::min(minV, v);
            maxV = glm::max(maxV, v);
        }
        glm::vec3 meshSize = maxV - minV;
        float meshVolume = meshSize.x * meshSize.y * meshSize.z;

        // AABB
        AABB aabb = AABB::ComputeAABBForGameObject(obj);
        float aabbVolume = aabb.getVolume();
        float aabbError = (aabbVolume - meshVolume) / meshVolume;

        // OBB
        OBB obb = OBB::ComputeOBBForGameObject(obj);
        float obbVolume = obb.getVolume();
        float obbError = (obbVolume - meshVolume) / meshVolume;

        std::ostringstream line;
        line << std::fixed << std::setprecision(5);
        line << "Object: " << obj->getName()
            << " | Mesh Vol: " << meshVolume
            << " | AABB Vol: " << aabbVolume
            << " | OBB Vol: " << obbVolume
            << " | AABB Wasted Space: " << aabbError * 100 << "%"
            << " | OBB Wasted Space: " << obbError * 100 << "%\n";
        std::cout << line.str();
        TestManager::WriteLine(line.str());
        // Percentages represent, wasted space +32% for example is when the collission box is 32% too big compared to the mesh -40% then its too small

    }
}
// ----------------------------
// PERFORMANCE = measure speed of computation over x amount of iterations
// ----------------------------
void TestManager::RunPerformanceTest(const std::vector<GameObject*>& objects) {
    std::cout << "\n--- PERFORMANCE TEST ---\n";
    const int iterations = 100;

    // AABB Computation Test
    auto aabbStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        for (auto obj : objects) {
            volatile AABB aabb = AABB::ComputeAABBForGameObject(obj); 
        }
    }
    auto aabbEnd = std::chrono::high_resolution_clock::now();
    float aabbDurationMs = std::chrono::duration_cast<std::chrono::milliseconds>(aabbEnd - aabbStart).count();

    // OBB Computation Test
    auto obbStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        for (auto obj : objects) {
            volatile OBB obb = OBB::ComputeOBBForGameObject(obj);
        }
    }
    auto obbEnd = std::chrono::high_resolution_clock::now();
    float obbDurationMs = std::chrono::duration_cast<std::chrono::milliseconds>(obbEnd - obbStart).count();

    // Results
    std::ostringstream line;
    line << "Results after " << iterations << " iterations:\n"
        << "  AABB Total Time: " << aabbDurationMs << " ms\n"
        << "  OBB Total Time: " << obbDurationMs << " ms\n"
        << "  OBB/AABB Time Ratio: " << obbDurationMs / aabbDurationMs << "x\n"
        << "  Avg Time per Object:\n"
        << "    AABB: " << aabbDurationMs / (iterations * objects.size()) << " ms\n"
        << "    OBB: " << obbDurationMs / (iterations * objects.size()) << " ms\n";

    std::cout << line.str();
    WriteLine(line.str());
}
// ----------------------------
// CONSISTENCY = Check how consistent collision boxes are
// ----------------------------
void TestManager::RunConsistencyTest(const std::vector<GameObject*>& objects) {
    std::cout << "\n--- CONSISTENCY TEST ---\n";
    std::cout << std::fixed << std::setprecision(4);

    for (GameObject* obj : objects) {
        if (!obj || !obj->getMesh()) continue;

        // ========================================================================
        // OBB Consistency Checks
        // ========================================================================
        //OBB obb1 = CollisionDetector(obj, obj).CreateOBBForGameObject(obj);
        OBB obb1 = OBB::ComputeOBBForGameObject(obj);
        OBB obb2 = OBB::ComputeOBBForGameObject(obj);
        //OBB obb2 = CollisionDetector(obj, obj).CreateOBBForGameObject(obj);

        float obbCenterDrift = glm::distance(obb1.center, obb2.center);
        float obbExtentsDrift = glm::distance(obb1.extents, obb2.extents);
        glm::quat obbRot1 = glm::quat_cast(obb1.orientation);
        glm::quat obbRot2 = glm::quat_cast(obb2.orientation);
        float obbAngleDrift = glm::degrees(glm::angle(obbRot1 * glm::inverse(obbRot2)));

        // ========================================================================
        // AABB Consistency Checks
        // ========================================================================
        AABB aabb1 = AABB::ComputeAABBForGameObject(obj);
        AABB aabb2 = AABB::ComputeAABBForGameObject(obj);

        float aabbCenterDrift = glm::distance(aabb1.GetCenter(), aabb2.GetCenter());
        float aabbExtentsDrift = glm::distance(aabb1.GetSize(), aabb2.GetSize());

        // ========================================================================
        // Results Output
        // ========================================================================
        std::ostringstream line;
        line << "Object: " << obj->getName() << "\n"
            << "  [OBB]\n"
            << "    Center Drift: " << obbCenterDrift << " units\n"
            << "    Extents Drift: " << obbExtentsDrift << " units\n"
            << "    Rotation Drift: " << obbAngleDrift << " degrees\n"
            << "  [AABB]\n"
            << "    Center Drift: " << aabbCenterDrift << " units\n"
            << "    Extents Drift: " << aabbExtentsDrift << " units\n"
            << "    (AABBs have no rotation drift)\n";

        std::cout << line.str();
        TestManager::WriteLine(line.str());
    }
}
// ----------------------------
// MEMORY  = Check memory usage between collision versions
// ----------------------------

void TestManager::RunMemoryTest(const std::vector<GameObject*>& objects) {
    std::cout << "\n--- MEMORY USAGE TEST ---\n";

    size_t aabbMemory = 0;
    size_t obbMemory = 0;

    for (auto obj : objects) {
        aabbMemory += sizeof(AABB::ComputeAABBForGameObject(obj));
        obbMemory += sizeof(OBB::ComputeOBBForGameObject(obj));
    }
    std::ostringstream line;

    line << "Total AABB Memory: " << aabbMemory << " bytes\n"
        << "Total OBB Memory: " << obbMemory << " bytes\n"
        << "OBB/AABB Memory Ratio: " << (float)obbMemory / aabbMemory << "x\n";

    std::cout << line.str();
    TestManager::WriteLine(line.str());
}

void TestManager::OpenFile(const std::string& filename) {
    testResults.open(filename);
    testResults << "TEST RESULTS AABB OBB PROTOTYPE\n";
}
void TestManager::WriteLine(const std::string& line) {
    if (testResults.is_open()) {
        testResults << line;
    }
}
void TestManager::CloseFile() {
    testResults.close();
}
