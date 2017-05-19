#include "FileLoader.h"

// inline MTLLoader functions
bool MTLLoader::loadMTL(const string path)
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

					if (material.kd.r>kEpsilon ||
						material.kd.g>kEpsilon ||
						material.kd.b>kEpsilon)
						material.diffuse = true;
				}
				else if (type == "Ka")
				{
					ss >> material.ka.r;
					ss >> material.ka.g;
					ss >> material.ka.b;

					if(material.ka.r>kEpsilon||
							material.ka.g>kEpsilon|| 
							material.ka.b>kEpsilon)
						material.self_luminous = true;
				}
				else if (type == "Ks")
				{
					ss >> material.ks.r;
					ss >> material.ks.g;
					ss >> material.ks.b;

					if (material.ks.r>kEpsilon ||
						material.ks.g>kEpsilon ||
						material.ks.b>kEpsilon)
						material.specular = true;
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
					break;	// new material		
			}			
			materialList.push_back(material);
		}				
	}	
	//Finally, reset to standard output again
	cin.rdbuf(cinbuf);
	return true;
}

bool MTLLoader::findMaterial(string materialName, Material &m)
{
	// find the Material according to its name
	for (int i = 0; i < materialList.size(); ++i)
		if (materialList[i].name == materialName)
		{
			m = materialList[i];
			return true;
		}
	return false;
}


// inline ObjLoader functions
//template<typename Out>
//void ObjLoader::split(const string &s, char delim, Out result) {
//	stringstream ss;
//	ss.str(s);
//	string item;
//	while (getline(ss, item, delim)) {
//		*(result++) = item;
//	}
//}

vector<int> ObjLoader::split(const string &s, char delim) {
	vector<int> elems;

	stringstream ss;
	ss.str(s);
	string item;
	while (getline(ss, item, delim)) {
		int i=stoi(item);
		elems.push_back(i);
	}
	return elems;
}

bool ObjLoader::loadObj(const string path, const string name)
{
	// step 1: test whether the file exists 
	ifstream in(path+name);
	if (in.fail())
	{
		cout << "The file does not exist!" << endl;
		return false;
	}

	streambuf *cinbuf = cin.rdbuf();	//save old buf
	cin.rdbuf(in.rdbuf());				//redirect cin to file

	// step 2: test whether the file is empty
	string line;
	getline(cin, line);
	if (cin.eof())
	{
		cout << "The file is empty!" << endl;
		return false;
	}

	// step 3: Load data
	string type; // the data type, e.g. vertex, texture or group...	
	Group *recentGroup=new Group;
	TriangleMesh *recentTriMesh=new TriangleMesh;

	bool smooth; // whether the face is smooth or not
	istringstream ss;
	MTLLoader mtlLoder;

	do
	{
		ss.str(line); ss.clear();
		ss >> type;
		if (type == "mtllib")
		{
			string name;
			ss >> name;
			// mtlLoder.loadMTL(path+name);
			mtlLoder.loadMTL(path+name);
		}
		else if (type == "g")
		{
			string name;
			ss >> name;
			if (name != "default")
			{
				recentGroup = new Group(name);
				scene.add(*(dynamic_cast<Object *> (recentGroup)));
			}
		}
		else if (type == "usemtl")
		{
			Material m;
			string materialName;
			ss >> materialName;

			if (!mtlLoder.findMaterial(materialName, m))
			{
				cout << "Don't have the material(ObjLoader)!" << endl;
				break;
			}

			recentTriMesh = new TriangleMesh(&m);
			recentGroup->add(*(dynamic_cast<Object *> (recentTriMesh)));
		}
		else if (type == "f")
		{
			vector<vector<int>> face;// (3 3 1) (4 4 2) (6 6 3) (5 5 4)
			while (!ss.eof())
			{
				string vertex;
				ss >> vertex;

				vector<int> x = split(vertex, '/');
				face.push_back(x);
			}

			for (int i = 0; i < face.size() - 2; ++i)
			{
				vector<Vec3f> vs;
				vs.push_back(vertexList[face[0][0]-1]);
				vs.push_back(vertexList[face[i + 1][0]-1]);
				vs.push_back(vertexList[face[i + 2][0] - 1]);

				vector<Vec2f> vt;
				vt.push_back(vertexTextureList[face[0][1] - 1]);
				vt.push_back(vertexTextureList[face[i + 1][1] - 1]);
				vt.push_back(vertexTextureList[face[i + 2][1] - 1]);

				vector<Vec3f> vn;
				vn.push_back(vertexNormalList[face[0][2] - 1]);
				vn.push_back(vertexNormalList[face[i + 1][2] - 1]);
				vn.push_back(vertexNormalList[face[i + 2][2] - 1]);

				Triangle tri(vs, vn, vt, smooth);
				recentTriMesh->addTriangle(tri);
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
		
		getline(cin, line);
	} while (!cin.eof());
	
	// step 4: Finally, reset to standard output again
	cin.rdbuf(cinbuf);
	return true;
}