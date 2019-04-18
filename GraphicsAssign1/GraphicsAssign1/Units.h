#pragma once

#include"Objects.h"
#include"MyDataType.h"

#define WORLDCOORDWINDOWWIDTH 1600
#define WORLDCOORDWINDOWHEIGHT 900

#define PLAYER_BOX_VELOCITY 10
#define ENEMY_BOX_VELOCITY 0.5

const Vector2 INITIAL_PLAYER_BOX_POSITION = Vector2(0, 0); //WORLDCOORDWINDOWWIDTH/16(100, 0)
const Vector2 INITIAL_ENEMY_BOX_POSITION = Vector2((7 * WORLDCOORDWINDOWWIDTH) / 8, 0);
const Vector2 INITIAL_BALL_POSITION = Vector2((15 * WORLDCOORDWINDOWWIDTH) / 32, (WORLDCOORDWINDOWHEIGHT * 7) / 10);
const Vector2  BOXVELOCITYTORIGHT = Vector2(1, 0);
const Vector2  BOXVELOCITYTOLEFT = Vector2(-1, 0);
const Vector2  BOXVELOCITYZERO = Vector2(0, 0);
const Vector2 INITIAL_LEFT_WALL_POSITION = Vector2(-10, 0);
const Vector2 INITIAL_RIGHT_WALL_POSITION = Vector2(WORLDCOORDWINDOWWIDTH, 0);
const Vector2 INITIAL_TOP_WALL_POSITION = Vector2(0, WORLDCOORDWINDOWHEIGHT);

static GameObjectTree* GetPlayer();

static GameObjectTree* GetEnemy();

static GameObjectTree* GetBall();

static GameObjectTree* GetWall();