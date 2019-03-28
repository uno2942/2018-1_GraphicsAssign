#pragma once
#include<vector>
#include"Objects.h"
#include"MyDataType.h"

enum CamMode { WHOLE, FOLLOWBALL };
extern CamMode camMode;
extern bool ReshapeFlag;

extern vector<BinaryTree<ObjectWithComponent, string>> objectsTreeVectorForDraw;