#include "SPHSim.h"
#include <Windows.h>
#include "Utils.h"

void SPHSimReader::loadSimulation(string input)
{
	fstream file;
	file.open(input);
	int frameCount = -1;
	double px, py, pz;
	while (!file.eof())
	{
		int x = 0;
		file >> x;
		if (x == 0) {

			frameCount++;
			sim.frame.push_back(vector<vec3>());
			//MessageBox(0, FloatToString(frameCount).c_str(), 0, 0);
			//MessageBox(0, FloatToString(py).c_str(), 0, 0);

		}



		file >> px >> py >> pz;
		//if (frameCount >= 100) 
		//break;

		sim.frame[frameCount].push_back(vec3(px, py, pz));



	}
}