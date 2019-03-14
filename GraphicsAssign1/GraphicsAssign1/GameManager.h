#pragma once
#include"Objects.h"
#include<list>
#include<utility>
#define THRESHOLDSCORE 13
#define WORLDCOORDWINDOWWIDTH 1600
#define WORLDCOORDWINDOWHEIGHT 900
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
		void CheckCollisionAtUpSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList);
		void CheckCollisionAtDownSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList);
		void CheckCollisionAtLeftSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList);
		void CheckCollisionAtRightSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList);
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

	GameManager(GameManager const&) = delete;
	void operator=(GameManager const&) = delete;

	Box playerBox = Box(WORLDCOORDWINDOWWIDTH/16, WORLDCOORDWINDOWHEIGHT/18, "player"); //(100, 50)
	Box enemyBox = Box(WORLDCOORDWINDOWWIDTH / 16, WORLDCOORDWINDOWHEIGHT / 18, "enemy");
	Box net = Box(WORLDCOORDWINDOWWIDTH / 32, WORLDCOORDWINDOWHEIGHT/2, "net");
	Box screen = Box(WORLDCOORDWINDOWWIDTH, WORLDCOORDWINDOWHEIGHT, "screen");
	Circle ball = Circle(100, 100);
private:
	GameManager();
//	GameManager(GameManager const&);   // Don't Implement
//	void operator=(GameManager const&); // Don't implement
	//Manager
	SceneManager sceneManager;
	CollisionManager collisionManager;

	//ObjectData

	int myScore = 0;
	int enemyScore = 0;
	bool oneGameEnd = false;
	bool wholeGameEnd = false;
	int WhoFinallyWin = 0;

	const Vector2 BOXVELOCITYTORIGHT = Vector2(1, 0);
	const Vector2 BOXVELOCITYTOLEFT = Vector2(-1, 0);
	const Vector2 BOXVELOCITYZERO = Vector2(0, 0);

	/* need to set(dummy data)*/
	const Vector2 INITIALPLAYERBOXPOSITION = Vector2(WORLDCOORDWINDOWWIDTH/16, 0); //(100, 0)
	const Vector2 INITIALENEMYBOXPOSITION = Vector2((7* WORLDCOORDWINDOWWIDTH)/8, 0);
	const Vector2 INITIALBALLPOSITION = Vector2(WORLDCOORDWINDOWWIDTH/2-50, (WORLDCOORDWINDOWHEIGHT*9)/10);
	const Vector2 INITIALNETPOSITION = Vector2(201, 0); //(31*WORLDCOORDWINDOWWIDTH)/64(775, 0)

	bool playerBoxMoveRightFlag = false;
	bool playerBoxMoveLeftFlag = false;

	void StartGame();
	void InitBallVelocity();
	void InitObjectsPosition();
	void SetplayerBoxVelocity();
	void SetObjectPosition();
	void OneGameEnd(bool whoWin);
};