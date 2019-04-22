#pragma once
#include<vector>
#include"Objects.h"
#include"MyDataType.h"

enum CamMode { CHARACTER, BEHIND, HANGING };
extern CamMode camMode;
extern bool ReshapeFlag;

extern vector<GameObjectTree> objectsTreeVectorForDraw;