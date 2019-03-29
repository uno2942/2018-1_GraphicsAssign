#pragma once
#include<vector>
#include "GameManager.h"
#include "Objects.h"
#include"CommonBetweenGameAndDraw.h"
void myReshape(int width, int height);

void display();

void drawObjectRecursive(GameObjectNode* root);

void representComponent(const Transform &object);

void representBox(const Transform& box);

void representCircle(const Transform& circle);

void representTriangle(const Transform& triangle);

void lookAtBall(const Object& circle);

void representScore(int score, GLfloat x, GLfloat y);

void representResult(void);