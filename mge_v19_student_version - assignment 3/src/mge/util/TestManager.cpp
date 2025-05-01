#include "mge/util/TestManager.hpp"
#include <chrono>
#include <iostream>
#include <random>


void TestManager::RunAllTests(const std::vector<GameObject*>& objects) {
    RunAccuracyTest(objects);
    RunPerformanceTest(objects);
    RunConsistencyTest(objects);
    //RunScalingTest(objects);
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

        glm::mat4 world = obj->getWorldTransform();
        glm::vec3 minV(FLT_MAX), maxV(-FLT_MAX);
        for (auto& v : mesh->_vertices) {
            glm::vec3 worldV = glm::vec3(world * glm::vec4(v, 1.0f));
            minV = glm::min(minV, worldV);
            maxV = glm::max(maxV, worldV);
        }
        glm::vec3 meshSize = maxV - minV;
        float meshVolume = meshSize.x * meshSize.y * meshSize.z;

        // AABB
        AABB aabb = AABB::ComputeLocalAABB(mesh);
        float aabbVolume = glm::compMax(aabb.max - aabb.min);
        float aabbError = (aabbVolume - meshVolume) / meshVolume;

        // OBB
        OBB obb = CollisionDetector(obj, obj).CreateOBBForGameObject(obj);
        float obbVolume = obb.getVolume();
        float obbError = (obbVolume - meshVolume) / meshVolume;


        std::cout << "Object: " << obj->getName()
            << " | AABB Wasted Space: " << aabbError * 100 << "%"
            << " | OBB Wasted Space: " << obbError * 100 << "%\n";
        // Percentages represent, wasted space +32% for example is when the collission box is 32% too big compared to the mesh -40% then its too small

    }
}
// ----------------------------
// PERFORMANCE = measure speed
// ----------------------------
void TestManager::RunPerformanceTest(const std::vector<GameObject*>& objects) {
    std::cout << "\n--- PERFORMANCE TEST (AABB vs OBB) ---\n";

    const int iterations = 1000;
    int aabbCollisions = 0;
    int obbCollisions = 0;

    // AABB Test
    auto aabbStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        for (size_t a = 0; a < objects.size(); ++a) {
            for (size_t b = a + 1; b < objects.size(); ++b) {
                AABB aabb1 = AABB::ComputeAABBForGameObject(objects[a]);
                AABB aabb2 = AABB::ComputeAABBForGameObject(objects[b]);
                if (CollisionDetector::checkCollisionAABB(aabb1, aabb2)) {
                    aabbCollisions++;
                }
            }
        }
    }
    auto aabbEnd = std::chrono::high_resolution_clock::now();
    float aabbDurationMs =
        std::chrono::duration_cast<std::chrono::milliseconds>(aabbEnd - aabbStart).count();

    // OBB Test
    auto obbStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        for (size_t a = 0; a < objects.size(); ++a) {
            for (size_t b = a + 1; b < objects.size(); ++b) {
                CollisionDetector cd(objects[a], objects[b]);
                if (cd.checkCollisionOBB()) {
                    obbCollisions++;
                }
            }
        }
    }
    auto obbEnd = std::chrono::high_resolution_clock::now();
    float obbDurationMs =
        std::chrono::duration_cast<std::chrono::milliseconds>(obbEnd - obbStart).count();

    // Results
    std::cout << "AABB Results:\n"
        << "  Collisions: " << aabbCollisions << "\n"
        << "  Total Time: " << aabbDurationMs << " ms\n"
        << "  Avg Time/Check: " << aabbDurationMs / (iterations * objects.size() * (objects.size() - 1)) << " ms\n\n";

    std::cout << "OBB Results:\n"
        << "  Collisions: " << obbCollisions << "\n"
        << "  Total Time: " << obbDurationMs << " ms\n"
        << "  Avg Time/Check: " << obbDurationMs / (iterations * objects.size() * (objects.size() - 1)) << " ms\n\n";

    std::cout << "OBB is " << obbDurationMs / aabbDurationMs << "x slower than AABB\n";
}
// ----------------------------
// CONSISTENCY = repeat and verify stability
// ----------------------------
void TestManager::RunConsistencyTest(const std::vector<GameObject*>& objects) {
    std::cout << "\n--- CONSISTENCY TEST (OBB Stability) ---\n";

    for (GameObject* obj : objects) {
        if (!obj) continue;

        // Generate OBBs twice to check consistency
        OBB obb1 = CollisionDetector(obj, obj).CreateOBBForGameObject(obj);
        OBB obb2 = CollisionDetector(obj, obj).CreateOBBForGameObject(obj);

        // Calculate differences
        float centerDiff = glm::distance(obb1.center, obb2.center);
        float extentsDiff = glm::distance(obb1.extents, obb2.extents);

        // Orientation difference
        glm::quat rot1 = glm::quat_cast(obb1.orientation);
        glm::quat rot2 = glm::quat_cast(obb2.orientation);
        float angleDiff = glm::degrees(glm::angle(rot1 * glm::inverse(rot2)));

        std::cout << "Object: " << obj->getName() << "\n"
            << "  Center Drift: " << centerDiff << " units\n"
            << "  Extents Drift: " << extentsDiff << " units\n"
            << "  Rotation Drift: " << angleDiff << " degrees\n";
    }
}