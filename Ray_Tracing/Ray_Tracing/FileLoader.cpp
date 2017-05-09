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

	// step 2: load the data according to its start character
	// case 1: v v1, v2, v3
	// case 2: f v1, v2, ...
	string head;
	bool not_quit = true;
	while (not_quit)
	{
		// read the head of line
		getline(cin, head);
		if (cin.eof())
			not_quit = false;// End Of File. Quit the loop.
		else
		{
			istringstream ss(head);
			string type;
			ss >> type;
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

					ss.str(content);

					ss >> type;
					if (type == "illum")					
						ss >> material.illum;					
					else if (type == "Kd")
					{
						Kd kd;
						ss >> kd.r;
						ss >> kd.g;
						ss >> kd.b;
						material.addAttribute(kd);
					}
					else if (type == "Ka")
					{
						Ka ka;
						ss >> ka.r;
						ss >> ka.g;
						ss >> ka.b;
						material.addAttribute(ka);
					}
					else if (type == "Ks")
					{
						Ks ks;
						ss >> ks.r;
						ss >> ks.g;
						ss >> ks.b;
						material.addAttribute(ks);
					}
					else if (type == "Ns")
					{
						Ns ns;
						ss >> ns.exponent;
						material.addAttribute(ns);
					}
					else if (type == "Tr")
					{
						Tr tr;
						ss >> tr.ratio;
						material.addAttribute(tr);
					}
					else if (type == "Ni")
					{
						Ni ni;
						ss >> ni.optical_density;
						material.addAttribute(ni);
					}
					else if (type == "Tf")
					{
						Tf tf;
						ss >> tf.r;
						ss >> tf.g;
						ss >> tf.b;
						material.addAttribute(tf);
					}
					else if (type == "Ke")
					{
						Ke ke;
						ss >> ke.r;
						ss >> ke.g;
						ss >> ke.b;
						material.addAttribute(ke);
					}
					else if (type == "newmtl")						
						break;	// new material					
				}
			}			
		}
	}
	cin.rdbuf(cinbuf);//reset to standard output again
	return true;
}