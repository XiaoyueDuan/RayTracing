#pragma once
#include"MultiObjects.h"

class Scene :MultiObjects
{
private:
	string name;

public:
	Scene(string name)
	{
		this->name = name;
	}
};