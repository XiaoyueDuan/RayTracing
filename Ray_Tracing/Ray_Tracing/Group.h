#pragma once
#include"MultiObjects.h"

class Group:MultiObjects
{
private: 
	string name;

public:
	Group(string name)
	{
		this->name = name;
	}
};