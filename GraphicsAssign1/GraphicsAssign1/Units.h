#pragma once

#include"Objects.h"
#include"MyDataType.h"

#define WORLD_COORD_MAP_XLEN 1600
#define WORLD_COORD_MAP_ZLEN 1600
#define WORLD_COORD_MAP_YLEN 900

#define PLAYER_BOX_VELOCITY 10
#define ENEMY_BOX_VELOCITY 0.5

const Vector3 INITIAL_PLAYER_BOX_POSITION = Vector3(WORLD_COORD_MAP_XLEN/2, WORLD_COORD_MAP_ZLEN - 300,0); //WORLDCOORDWINDOWWIDTH/16(100, 0)
const Vector3 INITIAL_ENEMY_BOX_POSITION = Vector3((7 * WORLD_COORD_MAP_XLEN) / 8, 0, 0);
const Vector3 INITIAL_BALL_POSITION = Vector3((15 * WORLD_COORD_MAP_XLEN) / 32, (WORLD_COORD_MAP_ZLEN * 7) / 10, 0);
const Vector3  BOXVELOCITYTORIGHT = Vector3(1, 0, 0);
const Vector3  BOXVELOCITYTOLEFT = Vector3(-1, 0, 0);
const Vector3  BOXVELOCITYZERO = Vector3(0, 0, 0);
const Vector3 INITIAL_LEFT_WALL_POSITION = Vector3(0, WORLD_COORD_MAP_ZLEN / 2, WORLD_COORD_MAP_YLEN / 2);
const Vector3 INITIAL_RIGHT_WALL_POSITION = Vector3(WORLD_COORD_MAP_XLEN, WORLD_COORD_MAP_ZLEN/2, WORLD_COORD_MAP_YLEN/2);
const Vector3 INITIAL_TOP_WALL_POSITION = Vector3(WORLD_COORD_MAP_XLEN/2, WORLD_COORD_MAP_ZLEN/2, WORLD_COORD_MAP_YLEN);

GameObjectTree* GetPlayer();
GameObjectTree* GetEnemy();
GameObjectTree* GetBall();
GameObjectTree* GetLeftWall();
GameObjectTree* GetRightWall();
GameObjectTree* GetFrontWall();
GameObjectTree* GetBackWall();
GameObjectTree* GetBottomWall();

void SetplayerBoxVelocity(bool playerBoxMoveRightFlag, bool playerBoxMoveLeftFlag);
void SetplayerBoxRotation(bool playerBoxRotateCounterClockwiseFlag, bool playerBoxRotateClockwiseFlag);

void SetenemyBoxVelocity(GLint enemyMoveTime, Object* ball);

void InitBallVelocity(GLint BALL_VELOCITY);