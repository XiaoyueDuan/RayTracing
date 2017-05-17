#pragma once
#include"MultiObjects.h"
#include"TriangleMesh.h"

class Group:public MultiObjects
{
private: 
	string name;

public:
	Group(string name)
	{
		this->name = name;
	}

	void setName(string name)
	{
		this->name = name;
	}
};