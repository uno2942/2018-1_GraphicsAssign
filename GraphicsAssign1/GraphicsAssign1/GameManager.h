#pragma once
#include"Objects.h"
#include<list>
#include<utility>
#define THRESHOLDSCORE 13
#define WORLDCOORDWINDOWWIDTH 1600
#define WORLDCOORDWINDOWHEIGHT 900
#define BALL_VELOCITY 300
#define PLAYER_BOX_VELOCITY 10;
using namespace std;
class GameManager {
public:
	class SceneManager {

	};

	class CollisionManager {
	public:
		CollisionManager();
		list<pair<pair<Object*, Object*>, Vector2>>* CollisionCheck();
		void CollisionHandler(list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList);
		bool CheckCollisionAtUpSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList);
		bool CheckCollisionAtDownSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList);
		bool CheckCollisionAtLeftSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList);
		bool CheckCollisionAtRightSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList);
		void CheckCollision4side(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList);
	private:
		list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList;
	};
	static GameManager& getInstance()
	{
		static GameManager instance;
		return instance;
	}

	/**
	Velocity¸¦ ¹Ù²ÜÁö positionÀ» ¹Ù²ÜÁö »ý°¢
	**/
	void OneFramePipeline();

	void SpecialKeyboardInputHandler(int key);
	void SavePlayerPositionBeforeReshape();
	void LoadPlayerPositionBeforeReshape();
	Vector2 playerBoxBeforeReshape;
	GameManager(GameManager const&) = delete;
	void operator=(GameManager const&) = delete;

	Box playerBox = Box(WORLDCOORDWINDOWWIDTH/8, WORLDCOORDWINDOWHEIGHT/18, "player"); //WORLDCOORDWINDOWWIDTH/16 (100, 50)
	Box enemyBox = Box(WORLDCOORDWINDOWWIDTH / 16, WORLDCOORDWINDOWHEIGHT / 18, "enemy");
	Box net = Box((WORLDCOORDWINDOWWIDTH) / 32, WORLDCOORDWINDOWHEIGHT/2, "net");
	Box leftwall = Box(10, WORLDCOORDWINDOWHEIGHT, "leftwall");
	Box rightwall = Box(10, WORLDCOORDWINDOWHEIGHT, "rightwall");
	Box topwall = Box(WORLDCOORDWINDOWWIDTH, 10, "topwall");
	Box screen = Box(WORLDCOORDWINDOWWIDTH, WORLDCOORDWINDOWHEIGHT, "screen");
	Circle ball = Circle(100, 100, "ball");
	int myScore = 0;
	int enemyScore = 0;
	int WhoFinallyWin = 0;

private:
	GameManager();
//	GameManager(GameManager const&);   // Don't Implement
//	void operator=(GameManager const&); // Don't implement
	//Manager
	SceneManager sceneManager;
	CollisionManager collisionManager;

	//ObjectData

	
	bool oneGameEnd = false;
	bool wholeGameEnd = false;
	
	int timeSinceStart = 0;
	int prevTime = 0;
	const Vector2 BOXVELOCITYTORIGHT = Vector2(1, 0);
	const Vector2 BOXVELOCITYTOLEFT = Vector2(-1, 0);
	const Vector2 BOXVELOCITYZERO = Vector2(0, 0);

	/* need to set(dummy data)*/
	const Vector2 INITIAL_PLAYER_BOX_POSITION = Vector2(0, 0); //WORLDCOORDWINDOWWIDTH/16(100, 0)
	const Vector2 INITIAL_ENEMY_BOX_POSITION = Vector2((7* WORLDCOORDWINDOWWIDTH)/8, 0);
	const Vector2 INITIAL_BALL_POSITION = Vector2(180, (WORLDCOORDWINDOWHEIGHT*7)/10);
	const Vector2 INITIAL_NET_POSITION = Vector2((31 * WORLDCOORDWINDOWWIDTH) / 64, 0); //(31*WORLDCOORDWINDOWWIDTH)/64(775, 0)
	const Vector2 INITIAL_LEFT_WALL_POSITION = Vector2(-10, 0);
	const Vector2 INITIAL_RIGHT_WALL_POSITION = Vector2(WORLDCOORDWINDOWWIDTH, 0);
	const Vector2 INITIAL_TOP_WALL_POSITION = Vector2(0, WORLDCOORDWINDOWHEIGHT);

	bool playerBoxMoveRightFlag = false;
	bool playerBoxMoveLeftFlag = false;
	bool ballLeftCollisionFlag = false;
	bool ballRightCollisionFlag = false;
	bool ballUpCollisionFlag = false;
	bool ballDownCollisionFlag = false;

	void StartGame();
	void InitializeGame();
	void InitBallVelocity();
	void InitObjectsPosition();
	void SetplayerBoxVelocity();
	void SetObjectPosition();
	void OneGameEnd(bool whoWin);
};