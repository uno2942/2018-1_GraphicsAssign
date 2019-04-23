#pragma once
#include<vector>
#include"Objects.h"
#include"MyDataType.h"

enum CamMode { CHARACTER, BEHIND, HANGING };
enum RenderingMode {NO_HIDDEN_LINE_REMOVAL, HIDDEN_LINE_REMOVAL};
extern CamMode camMode;
extern RenderingMode renMode;
extern bool ReshapeFlag;

extern vector<GameObjectTree> objectsTreeVectorForDraw;