#pragma once
#include<vector>
#include "Objects.h"
#include"CommonBetweenGameAndDraw.h"
void myReshape(int width, int height);

void display();

void representBox(const Box& box, int colorR, int colorG, int colorB);

void representCircle(const Oval& circle);

void lookAtBall(const Oval& circle);

void representScore(int score, GLfloat x, GLfloat y);

void representResult(void);