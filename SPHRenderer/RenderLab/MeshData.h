#pragma once
#include "Utils.h"
#include "OBJImporter.h"

class MeshData
{
	std::vector<Vertex> vertices;
	
public:

	bool importFromOBJ(string path);
	vector<Vertex> *getVertices();


};