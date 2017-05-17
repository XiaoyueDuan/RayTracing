#pragma once

// The description of file format from; http://paulbourke.net/dataformats/mtl/
#include <vector>
#include <string>

using namespace std;

class Attribute
{
public:
	Attribute() {}
};

class Kd :public Attribute
{
	//Kd r g b
	//	The Kd statement specifies the diffuse reflectivity using RGB values.

	//	"r g b" are the values for the red, green, and blue components of the
	//	atmosphere.The g and b arguments are optional.If only r is
	//	specified, then g, and b are assumed to be equal to r.The r g b values
	//	are normally in the range of 0.0 to 1.0.Values outside this range
	//	increase or decrease the relectivity accordingly.
public:
	float r, g, b;
	Kd() :Attribute() 
	{
		r = 0.0;
		g = 0.0;
		b = 0.0;
	}
};

class Ka :public Attribute
{
	//Ka r g b

	//	The Ka statement specifies the ambient reflectivity using RGB values.

	//	"r g b" are the values for the red, green, and blue components of the
	//	color.The g and b arguments are optional.If only r is specified,
	//	then g, and b are assumed to be equal to r.The r g b values are
	//	normally in the range of 0.0 to 1.0.Values outside this range increase
	//	or decrease the relectivity accordingly.
public:
	float r, g, b;
	Ka() :Attribute() 
	{
		r = 0.0;
		g = 0.0;
		b = 0.0;
	}
};

class Ks :public Attribute
{
	//Ks r g b
	//	The Ks statement specifies the specular reflectivity using RGB values.

	//	"r g b" are the values for the red, green, and blue components of the 
	//	atmosphere.  The g and b arguments are optional.  If only r is 
	//	specified, then g, and b are assumed to be equal to r.  The r g b values 
	//	are normally in the range of 0.0 to 1.0.  Values outside this range 
	//	increase or decrease the relectivity accordingly.
public:
	float r, g, b;
	Ks() :Attribute() 
	{
		r = 0.0;
		g = 0.0;
		b = 0.0;
	}
};

class Ns :public Attribute
{
	//Ns exponent
	//	Specifies the specular exponent for the current material.  This defines
	//	the focus of the specular highlight.

	//	"exponent" is the value for the specular exponent.  A high exponent
	//	results in a tight, concentrated highlight.  Ns values normally range
	//	from 0 to 1000.
public:
	int exponent;
	Ns() :Attribute() { exponent = 1; }
};

class Tr :public Attribute
{
	//Tr ratio
	//	Transparent ratio for the current material. If Tr=0, means it is fully opaque. 
	//	Higher value, more transpartent the material is. 
	//	Tr values normally range from 0.0 to 1.0.
public:
	float ratio;
	Tr() :Attribute() { ratio = 0; }
};

class Ni :public Attribute
{
	//Ni optical_density
	//	Specifies the optical density for the surface.This is also known as
	//	index of refraction.

	//	"optical_density" is the value for the optical density.The values can
	//	range from 0.001 to 10.  A value of 1.0 means that light does not bend
	//	as it passes through an object.Increasing the optical_density
	//	increases the amount of bending.Glass has an index of refraction of
	//	about 1.5. Values of less than 1.0 produce bizarre results and are not
	//	recommended.
public:
	float optical_density;
	Ni() :Attribute() { optical_density = 1.0; }
};

class Material
{
public:
	string name;
	int illum;
	Kd kd;
	Ka ka;
	Ks ks;
	Ns ns;
	Tr tr;
	Ni ni;

	bool self_luminous=false; 
	bool transparent=false;

	Material(){}
	Material(string name) 
	{
		this->name = name;
	}
};