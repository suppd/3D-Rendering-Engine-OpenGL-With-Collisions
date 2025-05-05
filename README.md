# 3D Rendering Engine OpenGL With Collisions

## Description & Justification
This project contains a simple open GL rendering engine thats able to render 3D meshes, texture them and feed shaders to the materials to for example have a debug shader for the collision boxes.
Further the project also contains AABB and OBB collision systems that are tested and benchmarked using the debug shader and test manager class.

Collision systems are a big part of the development of video games, there are many ways to handle collision detection and among them are the AABB & OBB methods. 
The common consensus is that AABB is cheap but less precise and OBB is expensive and more precise in this test we are going to see how they compare.

## Evaluation Proposal
"Benchmark accuracy, speed, memory usage, consistency, (scaling) performance OBB vs AABB colission systems in a simple OpenGL rendering engine"

## Turtorials Used

[AABB Tutorial](https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection)

[OBB Tutorial](https://github.com/juj/MathGeoLib/blob/master/src/Geometry/OBB.cpp)

[Chrono Library](https://en.cppreference.com/w/cpp/chrono)

[Writing To Text File](https://stackoverflow.com/questions/72493490/trying-creating-and-writing-into-a-txt-file-in-c)

## Test Environment

[CPU: Intel(R) Core(TM) i7-14700KF](https://www.intel.com/content/www/us/en/products/sku/236789/intel-core-i7-processor-14700kf-33m-cache-up-to-5-60-ghz/specifications.html)

[RAM: 32GB DDR4 3600HZ](https://azerty.nl/product/corsair-vengeance-lpx-geheugen/4069527)

[GPU: NVIDIA GeForce RTX 4070](https://www.gigabyte.com/Graphics-Card/GV-N4070GAMING-OC-12GD#kf)

[Motherboard: ASUS TUF GAMING B760M-PLUS WIFI D4](https://www.asus.com/motherboards-components/motherboards/tuf-gaming/tuf-gaming-b760m-plus-wifi-d4/)

OS: Windows 10 64-bit

[IDE: Microsoft Visual Studio Community 2022 (64-bit)](https://visualstudio.microsoft.com/vs/community/)

## Test Procedure

My testing procedure is setup within the code, I will run a few diffrent scenarios all with the same random seed (10399).
My Objective is to compare diffrent benchmarks between my AABB and OBB setups and see how well they do compared to each other.
The tests will all be recorded in a text file that is generated each test run using the file streaming library. I will then compile all data into a single file and then compare and analyze the results.
The tests are: 

10, 100 and 1000 iterations for all of these tests

•	Test With 100 Cubes 

•	Test With 100 Cubes Rotating 

•	Test With 100 Cubes Random Scale


•	Test With 100 Spheres

•	Test With 100 Spheres Rotating

•	Test With 100 Spheres Random Scale

Stress Tests:

•	Test with 100 Cubes 10.000 Iterations

•	Test with a 500 cubes (10 iterations only)



## Test Results

## Results Analysis

## Conclusion & Final Thoughts