#pragma once
#include<vector>
#include"Objects.h"
#include"MyDataType.h"

enum RenderingMode {NO_HIDDEN_LINE_REMOVAL, HIDDEN_LINE_REMOVAL};

extern RenderingMode renMode;
extern bool ReshapeFlag;

extern vector<GameObjectTree> objectsTreeVectorForDraw;