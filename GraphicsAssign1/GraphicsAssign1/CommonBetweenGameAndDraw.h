#pragma once
#include<vector>
#include"Objects.h"
#include"MyDataType.h"

enum CamMode { WHOLE, FOLLOWBALL };
CamMode camMode = WHOLE;
bool ReshapeFlag = false;

static vector<BinaryTree<ObjectWithComponent, string>> objectsTreeVectorForDraw;