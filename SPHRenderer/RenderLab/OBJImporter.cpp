#include "OBJImporter.h"

struct Import_Vertex
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 UV;
};

struct Import_Vertex_Comparator
{
	bool operator()(const Import_Vertex& v1, const Import_Vertex& v2)
	{
		if (v1.position.x < v2.position.x)
			return true;
		else if (v1.position.x > v2.position.x)
			return false;
		else
		{
			if (v1.position.y < v2.position.y)
				return true;
			else if (v1.position.y > v2.position.y)
				return false;
			else
			{
				if (v1.position.z < v2.position.z)
					return true;
				else if (v1.position.z > v2.position.z)
					return false;
				else
				{
					if (v1.normal.x < v2.normal.x)
						return true;
					else if (v1.normal.x > v2.normal.x)
						return false;
					else
					{
						if (v1.normal.y < v2.normal.y)
							return true;
						else if (v1.normal.y > v2.normal.y)
							return false;
						else
						{
							if (v1.normal.z < v2.normal.z)
								return true;
							else if (v1.normal.z > v2.normal.z)
								return false;
							else
							{
								if (v1.UV.x < v2.UV.x)
									return true;
								else if (v1.UV.x > v2.UV.x)
									return false;
								else
								{
									if (v1.UV.y < v2.UV.y)
										return true;
									else if (v1.UV.y > v2.UV.y)
										return false;
								}
							}

						}
					}
				}
			}
		}

		return v1.position.x < v2.position.x;
	}
};
bool loadOBJ(string path, vector<XMFLOAT3>& f_vertices, vector<XMFLOAT3>& f_normals, vector<XMFLOAT2>& f_UVs)
{

	f_vertices.clear();
	f_normals.clear();
	f_UVs.clear();

	vector<XMFLOAT3> vertices;
	vector<XMFLOAT3> normals;
	vector<XMFLOAT2> UVs;
	vector<Import_Vertex> iVertices;
	map<Import_Vertex, unsigned int, Import_Vertex_Comparator> vertices_mp;
	vector<float>vIndex, nIndex, uIndex;


	string data;
	ifstream file(path.c_str());

	if (!file.is_open())
	{
		MessageBox(0, "Couldn't find or open .obj model File", 0, 0);
		return false;
	}

	else

	{
		while (!file.eof())
		{
			getline(file, data);

			string temp = "";
			if (data[0] == 'v'&& data[1] != 't' && data[1] != 'n')
			{
				XMFLOAT3 ptstr;
				temp = "";
				bool neg[3] = { false, false, false };
				neg[0] = (data[2] == '-') ? true : false;
				for (int x = 0; x<(8 + neg[0]); x++)
				{
					temp += data[x + 2];
					neg[1] = (data[x + 4] == '-') ? true : false;

				}
				ptstr.x = StringToFloat(temp);

				temp = "";

				for (int x = 0; x<(8 + neg[1]); x++)
				{
					temp += data[10 + neg[0] + x + 1];
					neg[2] = (data[10 + neg[0] + x + 3] == '-') ? true : false;
				}
				ptstr.y = StringToFloat(temp);

				temp = "";

				for (int x = 0; x<(8 + neg[2]); x++)
				{
					temp += data[20 + neg[0] + neg[1] + x];
				}
				ptstr.z = -StringToFloat(temp);

				vertices.push_back(ptstr);

			}

			if (data[0] == 'v'&& data[1] == 't')
			{
				XMFLOAT2 lptsr;
				temp = "";
				for (int x = 0; x<8; x++)
				{
					temp += data[x + 3];
				}
				lptsr.x = StringToFloat(temp);

				temp = "";
				for (int x = 0; x<8; x++)
				{
					temp += data[x + 12];
				}
				lptsr.y = -StringToFloat(temp);
				lptsr.y = 1 - lptsr.y;

				UVs.push_back(lptsr);
			}

			if (data[0] == 'v'&& data[1] == 'n')
			{
				XMFLOAT3 ptst;
				temp = "";
				bool neg[3] = { false, false, false };
				neg[0] = (data[3] == '-') ? true : false;
				for (int x = 0; x<(8 + neg[0]); x++)
				{
					temp += data[x + 3];
					neg[1] = (data[x + 5] == '-') ? true : false;

				}
				ptst.x = StringToFloat(temp);

				temp = "";

				for (int x = 0; x<(8 + neg[1]); x++)
				{
					temp += data[10 + neg[0] + x + 2];
					neg[2] = (data[10 + neg[0] + x + 4] == '-') ? true : false;
				}
				ptst.y = StringToFloat(temp);

				temp = "";

				for (int x = 0; x<(8 + neg[2]); x++)
				{
					temp += data[21 + neg[0] + neg[1] + x];
				}
				ptst.z = StringToFloat(temp);

				normals.push_back(ptst);

			}

			if (data[0] == 'f')
			{
				int position = 2;

				for (int p = 0; p != 3; p++)
				{
					temp = "";
					while (IsNumber(data[position]))
					{
						temp += data[position];
						position++;

					}
					vIndex.push_back(StringToFloat(temp));
					temp = "";

					position++;
					while (IsNumber(data[position]))
					{
						temp += data[position];
						position++;
					}
					uIndex.push_back(StringToFloat(temp));
					position++;
					temp = "";

					while (IsNumber(data[position]))
					{
						temp += data[position];
						position++;
					}
					nIndex.push_back(StringToFloat(temp));


					position++;
					temp = "";
				}

			}

		}

	
		for (unsigned int i = (vIndex.size() - 1); i!=-1 ; i--)
		{
			/*Import_Vertex v;
			v.position = vertices[(unsigned int)vIndex[i] - 1];
			v.normal = normals[(unsigned int)nIndex[i] - 1];
			v.UV = UVs[(unsigned int)uIndex[i] - 1];
			vertices_mp.emplace(v , i);*/
			
			f_vertices.push_back(vertices[(unsigned int)vIndex[i] - 1]);
			f_UVs.push_back(UVs[(unsigned int)uIndex[i] - 1]);
			f_normals.push_back(normals[(unsigned int)nIndex[i] - 1]);
		}
		
		
	
		/*map<Import_Vertex, unsigned int , Import_Vertex_Comparator> sMp;
		vector<unsigned int> index_buffer;
		int count = 0;
		
		for (auto it = vertices_mp.begin(); it != vertices_mp.end(); it++) 
			sMp.emplace(it->first, count);

		for (int i = 0; i < f_vertices.size(); i++)
		{
			Import_Vertex temp;
			temp.position = f_vertices[i];
			temp.normal = f_normals[i];
			temp.UV = f_UVs[i];
			f_indices.push_back(sMp.find(temp)->second);
		}

		f_vertices.clear();
		f_UVs.clear();
		f_normals.clear();
		
		for (auto it = sMp.begin(); it != sMp.end(); it++)
		{
			f_vertices.push_back(it->first.position);
			f_normals.push_back(it->first.normal);
			f_UVs.push_back(it->first.UV);

		}*/
		
	}

	return true;

}