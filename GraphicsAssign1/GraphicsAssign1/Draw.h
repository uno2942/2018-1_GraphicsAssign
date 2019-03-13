#pragma once
#include"GameManager.h"

enum CamMode { WHOLE, FOLLOWBALL };

void myReshape(int width, int height, CamMode camMode);

void display();
