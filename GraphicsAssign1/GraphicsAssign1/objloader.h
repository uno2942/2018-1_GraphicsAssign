#pragma once
#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>


#include <glm/glm.hpp>
#include "ObjData.h"


// code source by https://github.com/opengl-tutorials/ogl/blob/master/common/objloader.cpp : OpenGL OFFICIAL TUTORIAL

bool loadOBJ(
	const char * path,
	ObjData* objData
);