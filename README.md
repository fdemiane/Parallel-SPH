# Parallel-SPH
Update: Outdated, a better version will be committed soon. The algorithms used are entirely different and one step towards a real-time simulation :)

A parallel implementation of Smooth Particle Hydrodynamics.
The project contains the code for the offline simulation and the code for a basic DirectX Renderer. The renderer is Windows only.
To run the Demo, press on the executable: You move with the WASD keys and rotate using the arrow keys.
The SPH folder contains the sequential part, the SPH-Parallel-Linux-Octree contains the parallel code and the SPHRenderer contains the source code for the renderer.
To use to SPH simulation, run the SPH parallel simulation on a Linux machine which takes the number of threads as argument (example ./SPH.exe 4), and copy the output.txt to the directory of the renderer. Failing to do so will crash the renderer. Run the renderer on a Windows 7,8 or 10 machine.
