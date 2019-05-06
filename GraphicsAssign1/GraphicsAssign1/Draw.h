#pragma once
#include<vector>
#include<cmath>
#include<algorithm>

#include "GameManager.h"
#include "Objects.h"
#include"CommonBetweenGameAndDraw.h"
#include "ObjData.h"

#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"objloader.h"
#include"myShader.h"
#include"Camera.h"
#define PI 3.1415926535

enum stringToInt { LEFTWALL, RIGHTWALL, FRONTWALL, BACKWALL, BOTTOMWALL, BALL, PLAYER, ENEMY };

extern map< string, int > mappingFromStringToInt;
extern map< string, Object* > mappingFromStringToUnit;
extern map<string, MyObjData*> ObjData_map;

void myReshape(int width, int height);
void display();
void genVAO(map< string, int > mappingFromStringToInt, map<string, MyObjData*>* ObjData_map, string ballObjPath, string playerObjPath, string enemyObjPath);
void addTexture(map< string, int > mappingFromStringToInt, map<string, MyObjData*>* ObjData_map, const vector<string> WallTexturePaths, const vector<string> PlayerTexturePaths); // bind .obj path for each object
