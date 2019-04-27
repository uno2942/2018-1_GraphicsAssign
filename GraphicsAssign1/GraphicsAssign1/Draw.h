#pragma once
#include<vector>
#include "GameManager.h"
#include "Objects.h"
#include"CommonBetweenGameAndDraw.h"
#include "ObjData.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
void myReshape(int width, int height);

void display();

//void drawObjectRecursive(GameObjectNode* root);

//void representComponent(const Transform &object);

//void representBox(const Transform& box);

//void representCircle(const Transform& circle);

//void representTriangle(const Transform& triangle);

void genWallVAO(const Transform* wall);

void genPolygonVAO(const Transform* object, string objPath);

ObjData* GetObj(string path);

//void lookAtBall(const Object& circle); //deleted in Assign#3

void representScore(int score, GLfloat x, GLfloat y);

void representResult(void);

void SetModelAndViewMatrix(CamMode camMode);

void SetHangingxy(int a, int b);