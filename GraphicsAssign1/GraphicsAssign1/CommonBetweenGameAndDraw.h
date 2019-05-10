#pragma once
#include<vector>
#include"Objects.h"
#include"MyDataType.h"

enum RenderingMode { GOURAUD, PHONG };

extern RenderingMode renMode;
extern bool ReshapeFlag;

extern vector<GameObjectTree> objectsTreeVectorForDraw;