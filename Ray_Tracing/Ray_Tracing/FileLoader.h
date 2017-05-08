#pragma once
#include <vector>
#include <string>

using namespace std;

class Attribute
{
public:
	Attribute(){}

	virtual void setValue(float){}
	virtual void setVaule(float []){}
};

class Kd :public Attribute
{
	// Kd r g b
	// diffuse reflectivity
private:
	
public:
	Kd():Attribute(){}
	virtual void setValue(float value[3])
	{

	}
};


class Material
{
private:
	vector<Attribute> attributeList;

public:
	void addAttribute(Attribute a)
	{
		attributeList.push_back(a);
	}
};

class MTLLoader
{
private:
	
};