#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include "MaterialDataStructure.h"

class MTLLoader
{
public:
	vector<Material> materialList;

	bool loadMTL(const char * path);
};