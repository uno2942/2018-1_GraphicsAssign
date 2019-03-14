#pragma once
#include"GameManager.h"

enum CamMode { WHOLE, FOLLOWBALL };

extern CamMode camMode;

void myReshape(int width, int height);

void display();

void representBox(const Box& box, int colorR, int colorG, int colorB);

void representCircle(const Circle& circle);

void lookAtBall(const Circle& circle);