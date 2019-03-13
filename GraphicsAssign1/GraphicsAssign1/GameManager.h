#pragma once
#include"Objects.h"
#include<list>
#include<utility>
using namespace std;
class GameManager {
public:
	class SceneManager {

	};

	class CollisionManager {
		list<pair<pair<Object, Object>, Vector2>>* CollisionCheck();
		void CollisionHandler(list<pair<pair<Object, Object>, Vector2>>* collisionPairList);
		Vector2 CheckCollisionAtUpSide(const Object& o1, const Object& o2);
		Vector2 CheckCollisionAtDownSide(const Object& o1, const Object& o2);
		Vector2 CheckCollisionAtLeftSide(const Object& o1, const Object& o2);
		Vector2 CheckCollisionAtRightSide(const Object& o1, const Object& o2);
		Vector2* CheckCollision4side(const Object& o1, const Object& o2);
	};
	static GameManager& getInstance()
	{
		static GameManager instance;
		return instance;
	}

	/**
	Velocity¸¦ ¹Ù²ÜÁö positionÀ» ¹Ù²ÜÁö »ý°¢
	**/
	void SpecialKeyboardInputHandler(int key);

	GameManager(GameManager const&) = delete;
	void operator=(GameManager const&) = delete;
private:
	GameManager();
	GameManager(GameManager const&);   // Don't Implement
	void operator=(GameManager const&); // Don't implement
	//Manager
	SceneManager sceneManager;
	CollisionManager collisionManager;

	//ObjectData
	Box playerBox;
	Box enemyBox;
	Box net;
	Box screen;
	Circle ball;

	int myScore = 0;
	int enemyScore = 0;
	bool gameEnd = false;
	const Vector2 BOXVELOCITYTORIGHT = Vector2(1, 0);
	const Vector2 BOXVELOCITYTOLEFT = Vector2(-1, 0);
	const Vector2 BOXVELOCITYZERO = Vector2(0, 0);

	/* need to set(dummy data)*/
	const Vector2 INITIALPLAYERBOXPOSITION = Vector2(1, 0);
	const Vector2 INITIALENEMYBOXPOSITION = Vector2(-1, 0);
	const Vector2 INITIALBALLPOSITION = Vector2(0, 0);
	const Vector2 INITIALNETPOSITION = Vector2(100, 0);

	bool playerBoxMoveRightFlag = false;
	bool playerBoxMoveLeftFlag = false;

	void StartGame();
	void InitBallVelocity();
	void InitObjectsPosition();
	void SetplayerBoxVelocity();
	void SetObjectPosition();
	void GameEnd(bool whoWin);

};