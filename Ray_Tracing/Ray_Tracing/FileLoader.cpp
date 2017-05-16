#include "FileLoader.h"

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
				}
				else if (type == "Ni")
				{
					ss >> material.ni.optical_density;
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

bool MTLLoader::findMaterial(string materialName, Material &m)
{
	// find the Material according to its name

	return false;
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

	string type;
	Scene scene;
	bool smooth;
	while (true)
	{
		getline(cin, head);
		if (cin.eof())
			break;
		
		istringstream ss(head);
		if (head.empty())
			continue;
		else
		{
			ss >> type;
			if (type=="mtllib")
			{
				MTLLoader mtlLoder;

				string name;
				ss >> name;
				// mtlLoder.loadMTL(path+name);
				mtlLoder.loadMTL(path);
			}
			else if (type == "g")
			{
				string name;
				ss >> name;

				if(name!="default")
					Group g(name);


				
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
	}

	//Finally, reset to standard output again
	cin.rdbuf(cinbuf);
	return true;
}