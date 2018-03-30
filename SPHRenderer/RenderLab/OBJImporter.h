#pragma once

#include "Utils.h"
#include<fstream>
#include <map>


using namespace std;
using namespace DirectX;


bool loadOBJ(string path, vector<XMFLOAT3>& f_vertices, vector<XMFLOAT3>& f_normals, vector<XMFLOAT2>& f_UVs);
