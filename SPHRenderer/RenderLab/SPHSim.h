#pragma once
#include <vector>
#include <fstream>
#include <string>

using namespace std;

struct vec3
{
	double x;
	double y;
	double z;

	vec3()
	{
		x = y = z = 0;
	}

	vec3(double a, double b, double c)
	{
		x = a;
		y = b;
		z = c;
	}
};
struct SPHSim
{
	std::vector< vector<vec3> > frame;
};

class SPHSimReader
{

public:
	SPHSim sim;
	void loadSimulation(string input);
};