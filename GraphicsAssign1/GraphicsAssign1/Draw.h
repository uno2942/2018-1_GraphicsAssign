#pragma once
#include<vector>
#include "GameManager.h"
#include "Objects.h"
#include"CommonBetweenGameAndDraw.h"
#include "ObjData.h"
void myReshape(int width, int height);

void display();

void drawObjectRecursive(GameObjectNode* root);

void representComponent(const Transform &object);

//void representBox(const Transform& box);

//void representCircle(const Transform& circle);

//void representTriangle(const Transform& triangle);

void representWall(const Transform& wall);

void representPolygon(const Transform& polygon);

ObjData* GetObj(Object::Shape shape);

void lookAtBall(const Object& circle);

void representScore(int score, GLfloat x, GLfloat y);

void representResult(void);