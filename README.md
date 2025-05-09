# 3D Rendering Engine OpenGL With Collision Box Computation For 3D Objects (No Collision Solving)

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

100 and 1000 iterations for all of these tests and random scales (between 0.5 - 1.5)

•	Test With 100 Cubes 

•	Test With 100 Cubes Rotated 

•	Test With 100 Spheres

•	Test With 100 Spheres Rotated

What is being tested per catagory?

Accuracy: Testing how well the generated collision box (AABB&OBB) fits around the target mesh and how much space is "wasted" meaning that if 10% space is wasted the box is 10% bigger than the actual mesh.
And -10% would mean it's too small, how i calculate this is: (collision box volume - mesh volume / mesh volume).

Speed: For the speed testing i basically run a loop with x amount of iteration (this is what 100_ITE / 1000_ITE means), over the randomly generated objects and their corresponding AABB & OBB box and check per collision box how long it takes to compute.

Consistency: The way I test consistency is again with a loop for each passed gameobject but this time i generate 2 boxes of both collision types per object and check the numerical diffrences between them if there are any, to see how consistent the generating is.

Memory Usage: I check the size of the generated boxes in bytes, and then compare them between AABB & OBB.

## Test Results
These test results consist of the following categories: accuracy, performance, consistency and memory.
I have a diffrent text file for each single test and iteration amount I used AI to summarize these as it would be impossible to do this as a human.
The raw test result files are included in the git repo, under assets > test results. With the following naming convention: (Number of object_3D object type_Number of Iterations_ITE meaning iterations_Rotated or not if empty)

## Cube Section

### Cube Graphs Containing Accuracy & Performance:
![alt text](https://github.com/suppd/3D-Rendering-Engine-OpenGL-With-Collisions/blob/main/Project/assets/Test%20Result%20Graphs/Cubes.png "Cubes Graph")

#### Consistency:

For the consistency test I originally was printing a lot of lines but decided to implement a threshold so i got rid of all 0 values since theyre not really relevant.

Non-Rotated Cubes (100_CUBES_*_ITE.txt):

OBB Drifts: 0° no drifts.

AABB Drifts: 0° no drifts.

Rotated Cubes (100_CUBES_*_ITE_ROTATED.txt):

OBB Drifts: Minor rotation drifts (max 0.056°, avg ~0.04°) in 7–9/100 cubes.

AABB Drifts: 0° no drifts

#### Memory:

In all of the tests the memory consumption was the same for all iterations

Total AABB Memory: 2400 bytes

Total OBB Memory: 6000 bytes

OBB/AABB Memory Ratio: 2.5x (OBB uses 2.5x more memory)

## Sphere Section

### Sphere Graphs Containing Accuracy & Performance:
![alt text](https://github.com/suppd/3D-Rendering-Engine-OpenGL-With-Collisions/blob/main/Project/assets/Test%20Result%20Graphs/Spheres.png "Spheres Graph")

#### Consistency:

Non-Rotated Spheres (100_SPHERES_*_ITE.txt):

OBB & AABB Drifts: 0° no drifts.

Rotated Spheres (100_SPHERES_*_ITE_ROTATED.txt):

OBB Drifts: Tiny rotation drifts (max 0.056°) in a few objects.

AABB Drifts: 0° no drifts.

#### Memory:

In all of the tests the memory consumption was the same for all iterations

Total AABB Memory: 2400 bytes

Total OBB Memory: 6000 bytes

OBB/AABB Memory Ratio: 2.5x (OBB uses 2.5x more memory)

#### 10.000 Cubes To Test Scalability:

I also ran two test with 10.000 cubes rotated and not rotated but only one iteration, the reason why i seperate these is because these text files were so long that they were too big for AI to read.
So I analyzed them myself roughly and found that all results were for the most part the same no rotation = both accurate with little deviancy, rotation = OBB accurate and AABB very varyingly accurate.
The only results that were intresting are these:

Results after 1 iterations:
  AABB Total Time: 120 ms
  OBB Total Time: 99 ms
  OBB/AABB Time Ratio: 0.825x
  Avg Time per Object:
    AABB: 0.012 ms
    OBB: 0.0099 ms
	
Total AABB Memory: 240000 bytes
Total OBB Memory: 600000 bytes
OBB/AABB Memory Ratio: 2.5x


Results after 1 iterations:
  AABB Total Time: 123 ms
  OBB Total Time: 100 ms
  OBB/AABB Time Ratio: 0.813008x
  Avg Time per Object:
    AABB: 0.0123 ms
    OBB: 0.01 ms
	
Total AABB Memory: 240000 bytes
Total OBB Memory: 600000 bytes
OBB/AABB Memory Ratio: 2.5x

	
## Conclusion & Final Thoughts

AABBs are the more memory-efficient and computationally stable option, with zero observed drifts in all test cases. However, the biggest downside is their bad accuracy on rotated objects, leading to high wasted space (up to ~76% for cubes and ~70% for spheres). This makes them unusable for precise collision detection in dynamic environments where objects frequently rotate.

OBBs, while 2.5x more memory-intensive, are more consistent, give near-perfect fits (0% wasted space), and are 16–17% faster in performance for cubes and ~7% faster for spheres. The minor rotation drifts observed (max 0.056°) are negligible in practice and likely happen because of floating point errors and not because of the OBB algorithm itself. Their better accuracy and speed make them the better fit for precise 3D collision box calculations, especially if objects are rotated.

Interestingly, the spheres performed better in consistency than the cubes. But both shapes followed the same memory and performance patterns as in the cube tests. Once again, it means that OBB is clearly the better option when it comes to accuracy and speed, while AABBs remain usable for static or axis-aligned objects where memory is a constraint.

Scalability wise both AABB and OBB perform as before in terms of accuracy, performance and consistency except the amount of memory that is used, it increases linearly.

So, in conclusion, OBB uses more memory than AABB no matter the shape but is a little bit faster than AABB to my surprise. And OBB is more precise when it comes to rotated objects generating a tight fit around the 3D object, which is also visualized in the scene: 

![alt text](https://github.com/suppd/3D-Rendering-Engine-OpenGL-With-Collisions/blob/main/Project/assets/RenderIMG.png "Scene Render")

This project validates the theoretical expectations of both collision systems while giving workable metrics. If I had more time or decided to work more on this project later on I would choose to work on a hybrid version of AABB/OBB, and test GPU-based collision handling to see how that would affect performance. I also would run more different types of tests.
