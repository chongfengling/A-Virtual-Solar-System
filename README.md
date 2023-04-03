# A Virtual Solar System

This is the starting repository for assignment 2 of PHAS0100: Research Computing with C++. You may add or remove C++ files in any directory. You should organise the files as you see fit but do read the Folder Structure section below to understand the intended use of the existing folders.

## Installing dependencies

We are using the package manager Conan to install the dependencies Catch2 and Eigen. In order to use CMake's `Release` target for performance and `Debug` for debugging, the libraries must be installed twice with:

```
conan install . --output-folder=build --build=missing -s build_type=Debug
conan install . --output-folder=build --build=missing -s build_type=Release
```

If you delete the `build` directory to clean your build, you may have to install the dependencies again.

## Building

To build from the project root directory you should run:

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

If you wish to debug your code, you should replace `Release` with `Debug`. For performance measurements, ensure you have built with the `Release` target.

## Testing

Once the project has been built, it can be tested by running:

```
cd build
ctest
```

## Folder structure

The project is split into four main parts aligning with the folder structure described in [the relevant section in Modern CMake](https://cliutils.gitlab.io/modern-cmake/chapters/basics/structure.html):

- `app/` contains all code implementing the command-line application.
- `lib/` contains all non-app code. Only code in this directory can be accessed by the unit tests.
- `include/` contains all `.hpp` files.
- `test/` contains all unit tests.

You are expected to edit the `CMakeLists.txt` file in each folder to add or remove sources as necessary. For example, if you create a new file `test/particle_test.cpp`, you must add `particle_test.cpp` to the line `add_executable(tests test.cpp)` in `test/CMakeLists.txt`. Please ensure you are comfortable editing these files well before the submission deadline. If you feel you are struggling with the CMake files, please see the Getting Help section of the assignment instructions.

## Usage Instructions

You should fill in the instructions for using the app here.

## Credits

This project is maintained by Dr. Jamie Quinn as part of UCL ARC's course, Research Computing in C++.

## Results
### 1.3 A Simulation of the Solar System in one year
**Task: Solar System**
- Time step (dt): 1e-05 years/2Pi
- Total time: 1 year
- Step count: 628318
- Running time: 631 ms
- Time per step: 0.00100427 ms

**Initial position of particles:**
- Particle 0 (Sun): 0, 0, 0
- Particle 1 (Mercury): 0.347697, -0.197754, 0
- Particle 2 (Venus): 0.534739, -0.451724, 0
- Particle 3 (Earth): 0.994237, 0.1072, 0
- Particle 4 (Mars): -1.27357, 0.792483, 0
- Particle 5 (Jupiter): -1.60114, 4.94736, 0
- Particle 6 (Saturn): -0.154988, 9.49874, 0
- Particle 7 (Uranus): 16.0581, 10.5252, 0
- Particle 8 (Neptune): 23.2942, -19.0628, 0

**Final position of particles after 1 year with dt=1e-05:**
- Particle 0 (Sun): -0.000316124, 0.000706206, 0
- Particle 1 (Mercury): 0.264261, -0.300355, 0
- Particle 2 (Venus): -0.578638, -0.39422, 0
- Particle 3 (Earth): 0.99411, 0.107346, 0
- Particle 4 (Mars): 1.44218, -0.410962, 0
- Particle 5 (Jupiter): -3.88201, 3.45985, 0
- Particle 6 (Saturn): -2.1743, 9.24719, 0
- Particle 7 (Uranus): 15.2279, 11.694, 0
- Particle 8 (Neptune): 24.0024, -18.1629, 0

**Screenshot**
![screenshot](./assets/1_3_screenshot.png)

### 2.2 Benchmarking the simulation
Energy changed and time costed after 100 years of simulation with different time step dt
#### 2.2.1 With compiler optimization (8 different dt, total time: 100 years, number of step: 200 $\pi$ / dt)
| dt | Running time (ms) | Time per step (ms) | Energy change | Number of step |
| --- | --- | --- | --- | --- |
| 0.01 | 68 | 0.00108227 | 9.92208e-06 | 62831 |
| 0.001 | 612 | 0.000974029 | 2.61235e-06 | 628318 |
|0.0005 | 1251 | 0.000995514 |1.83367e-06 |1256637 |
|0.0001 | 6104 | 0.000971482 | 7.21491e-07 | 6283185 |
|0.00005 | 12485 | 0.000993525 | 4.41891e-07 | 12566370 | 
|0.00001 | 61652 | 0.000981222 | 1.16686e-07| 62831853 |
|0.000005 | 123166 | 0.000980124 | 6.177e-08 | 125663706 |
|0.000001 | 621710 | 0.000989482 | 1.30497e-08 | 628318530 |

**Screenshot (dt=0.0001)**

![screenshot2](./assets/2_2_1_1e-4.png)

#### 2.2.2 Without compiler optimization
| dt | Running time (ms) | Time per step (ms) | Energy change | Number of step |
| --- | --- | --- | --- | --- |
| 0.0001 | 311168 | 0.0495239 | 7.21582e-07 | 6283185 |

**Screenshot (dt=0.0001)**
![screenshot2](./assets/2_2_2_1e-4.png)

#### 2.2.3 Summary

Given a fixed total time, The running time and energy change are linearly proportional to the time step dt while the running time per step remains constant (around 0.00099 ms). Compiler optimization results in a significant fiftyfold improvement in runtime (reduce from 0.048 ms to 0.001 ms, with dt=0.0001) and don't affect the energy change.

To balance the time efficiency and computational accuracy, I choose dt=0.00005 as the time step for further simulation.

### 2.3 Increasing the scale of the system
With dt=0.00005 and total time=100 years, the simulation results of the random initial conditions with different number of particles are shown below. The number of particles 8, 64, 256, 1024, 2048


