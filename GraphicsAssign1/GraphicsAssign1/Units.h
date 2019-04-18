#pragma once

#include"Objects.h"
#include"MyDataType.h"

#define WORLDCOORDWINDOWWIDTH 1600
#define WORLDCOORDWINDOWHEIGHT 900

#define PLAYER_BOX_VELOCITY 10
#define ENEMY_BOX_VELOCITY 0.5

const Vector3 INITIAL_PLAYER_BOX_POSITION = Vector3(0, 0); //WORLDCOORDWINDOWWIDTH/16(100, 0)
const Vector3 INITIAL_ENEMY_BOX_POSITION = Vector3((7 * WORLDCOORDWINDOWWIDTH) / 8, 0);
const Vector3 INITIAL_BALL_POSITION = Vector3((15 * WORLDCOORDWINDOWWIDTH) / 32, (WORLDCOORDWINDOWHEIGHT * 7) / 10);
const Vector3  BOXVELOCITYTORIGHT = Vector3(1, 0);
const Vector3  BOXVELOCITYTOLEFT = Vector3(-1, 0);
const Vector3  BOXVELOCITYZERO = Vector3(0, 0);
const Vector3 INITIAL_LEFT_WALL_POSITION = Vector3(-10, 0);
const Vector3 INITIAL_RIGHT_WALL_POSITION = Vector3(WORLDCOORDWINDOWWIDTH, 0);
const Vector3 INITIAL_TOP_WALL_POSITION = Vector3(0, WORLDCOORDWINDOWHEIGHT);

GameObjectTree* GetPlayer();
GameObjectTree* GetEnemy();
GameObjectTree* GetBall();
GameObjectTree* GetWall();

void SetplayerBoxVelocity(bool playerBoxMoveRightFlag, bool playerBoxMoveLeftFlag);

void SetenemyBoxVelocity(int enemyMoveTime, Object* ball);

void InitBallVelocity(int BALL_VELOCITY);