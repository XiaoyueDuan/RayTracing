#include "FileLoader.h"

// inline MTLLoader functions
bool MTLLoader::loadMTL(const char * path)
{
	// step 1: test whether the file exists 
	ifstream in(path);
	if (in.fail())
	{
		cout << "The file does not exist!" << endl;
		return false;
	}

	streambuf *cinbuf = cin.rdbuf();	//save old buf
	cin.rdbuf(in.rdbuf());				//redirect cin to file

	// step 2: test whether the file is empty
	string head;
	getline(cin, head);
	if (cin.eof())
	{
		cout << "The file is empty!" << endl;
		return false;
	}

	// step 3: the first line not starting with newmtl(format error)
	bool not_quit = true;
	istringstream ss(head);
	string type;
	ss >> type;
	if (type != "newmtl")
	{
		cout << "The format is not correct! Please check it!" << endl;
		return false;
	}

	// step 4: load materials
	while (not_quit)
	{
		if (type == "newmtl")
		{
			Material material;
			ss >> material.name;

			while (true)
			{
				// read the attributes of the material
				string content;
				getline(cin, content);

				if (cin.eof())
				{
					not_quit = false; // End Of File. Quit the loop.
					break;
				}

				ss.str(content); ss.clear();
				ss >> type;
				if (type == "illum")
					ss >> material.illum;
				else if (type == "Kd")
				{
					ss >> material.kd.r;
					ss >> material.kd.g;
					ss >> material.kd.b;
				}
				else if (type == "Ka")
				{
					ss >> material.ka.r;
					ss >> material.ka.g;
					ss >> material.ka.b;

					material.self_luminous = true;
				}
				else if (type == "Ks")
				{
					ss >> material.ks.r;
					ss >> material.ks.g;
					ss >> material.ks.b;
				}
				else if (type == "Ns")
				{
					ss >> material.ns.exponent;
				}
				else if (type == "Tr")
				{
					ss >> material.tr.ratio;
					material.transparent = true;
				}
				else if (type == "Ni")
				{
					ss >> material.ni.optical_density;
					material.transparent = true;
				}
				else if (type == "newmtl")
				{
					materialList.push_back(material);
					break;	// new material		
				}
			}
		}
	}
	//Finally, reset to standard output again
	cin.rdbuf(cinbuf);
	return true;
}

bool MTLLoader::findMaterial(string materialName, Material *m)
{
	// find the Material according to its name

	return false;
}


// inline ObjLoader functions
template<typename Out>
void ObjLoader::split(const string &s, char delim, Out result) {
	stringstream ss;
	ss.str(s);
	string item;
	while (getline(ss, item, delim)) {
		*(result++) = item;
	}
}

vector<int> ObjLoader::split(const string &s, char delim) {
	vector<int> elems;
	split(s, delim, back_inserter(elems));
	return elems;
}

bool ObjLoader::loadObj(const char * path, const char * fileName)
{
	// step 1: test whether the file exists 
	ifstream in(path);
	//ifstream in(path + fileName);
	if (in.fail())
	{
		cout << "The file does not exist!" << endl;
		return false;
	}

	streambuf *cinbuf = cin.rdbuf();	//save old buf
	cin.rdbuf(in.rdbuf());				//redirect cin to file

	// step 2: test whether the file is empty
	string head;
	getline(cin, head);
	if (cin.eof())
	{
		cout << "The file is empty!" << endl;
		return false;
	}

	string type; // the data type, e.g. vertex, texture or group...
	Scene scene; // the Scene object
	bool smooth; // whether the face is smooth or not
	istringstream ss;
	MTLLoader mtlLoder;
	while (true)
	{
		getline(cin, head);
		if (cin.eof())
			break;		
		if (head.empty())
			continue;

		ss.str(head); ss.clear();
		ss >> type;
		if (type == "mtllib")
		{
			string name;
			ss >> name;
			// mtlLoder.loadMTL(path+name);
			mtlLoder.loadMTL(path);
		}
		else if (type == "g")
		{
			string name;
			ss >> name;

			if (name != "default")
			{
				Group g(name);
				scene.add(*(dynamic_cast<Object *> (&g)));

				do
				{
					ss.str(head); ss.clear();
					ss >> type;
					if (type == "usemtl")
					{
						Material m;
						string materialName;
						ss >> materialName;
						if (!mtlLoder.findMaterial(materialName, &m))
						{
							cout << "Don't have the material!" << endl;
							break;//只能跳出一层循环，有问题，以后改
						}
						TriangleMesh triMesh(&m);

						while (true)
						{
							getline(cin, head);
							ss.str(head); ss.clear();
							ss >> type;

							if (type == "usemtl")
								break;
							else if (type == "f")
							{
								vector<vector<int>> face;// (3 3 1) (4 4 2) (6 6 3) (5 5 4)
								while (!ss.eof())
								{
									string vertex;
									ss >> vertex;

									vector<int> x=split(vertex, '/');
									face.push_back(x);
								}	

								for (int i = 0; i < face.size() - 2; ++i)
								{
									vector<Vec3f> vs; 
									vs.push_back(vertexList[face[0][0]]);
									vs.push_back(vertexList[face[i+1][0]]);
									vs.push_back(vertexList[face[i + 2][0]]);

									vector<Vec3f> vn; 
									vn.push_back(vertexNormalList[face[0][1]]);
									vn.push_back(vertexNormalList[face[i + 1][1]]);
									vn.push_back(vertexNormalList[face[i + 2][1]]);

									vector<Vec2f> vt;
									vt.push_back(vertexTextureList[face[0][2]]);
									vt.push_back(vertexTextureList[face[i + 1][2]]);
									vt.push_back(vertexTextureList[face[i + 2][2]]);

									Triangle tri(vs, vn, vt, smooth);
									triMesh.addTriangle(tri);
								}
							} 
						}
						g.add(*dynamic_cast<Object *> (&triMesh));
					}
					getline(cin, head);
				} while (cin.eof());
			}
		}
		else if (type == "v")
		{
			Vec3f vertex;
			ss >> vertex.x;
			ss >> vertex.y;
			ss >> vertex.z;
			vertexList.push_back(vertex);
		}
		else if (type == "vt")
		{
			Vec2f vertexTexture;
			ss >> vertexTexture.x;
			ss >> vertexTexture.y;
			vertexTextureList.push_back(vertexTexture);
		}
		else if (type == "vn")
		{
			Vec3f vertexNormal;
			ss >> vertexNormal.x;
			ss >> vertexNormal.y;
			ss >> vertexNormal.z;
			vertexNormalList.push_back(vertexNormal);
		}
		else if (type == "s")
		{
			string off;
			ss >> off;
			if (off == "off")
				smooth = false;
			else
				smooth = true;
		}

	}

	//Finally, reset to standard output again
	cin.rdbuf(cinbuf);
	return true;
}