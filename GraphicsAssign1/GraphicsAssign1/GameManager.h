#pragma once
#include<vector>
#include<map>
#include<utility>
#include"Objects.h"
#include"CommonBetweenGameAndDraw.h"

#define THRESHOLDSCORE 13
#define WORLDCOORDWINDOWWIDTH 1600
#define WORLDCOORDWINDOWHEIGHT 900
#define PLAYER_BOX_VELOCITY 10
#define ENEMY_BOX_VELOCITY 0.5
#define YBORDER 0
using namespace std;

class GameManager {
public:
	class SceneManager {

	};

	class CollisionManager {
	public:
		CollisionManager();
		~CollisionManager();
		vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>* CollisionCheck();
		void CollisionHandler(vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>* collisionPairvector);
		vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>* RestoreBallPosition(vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>* collisionPairvector);
		bool CheckCollisionAtUpSide(const CollisionComponent& o1, const CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>* collisionPairvector);
		bool CheckCollisionAtDownSide(const CollisionComponent& o1, const CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>* collisionPairvector);
		bool CheckCollisionAtLeftSide(const CollisionComponent& o1, const CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>* collisionPairvector);
		bool CheckCollisionAtRightSide(const CollisionComponent& o1, const CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>* collisionPairvector);
		void CheckCollision4side(const CollisionComponent& o1, const CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>* collisionPairvector);
		GLdouble ballDeltaTime = 0;

		static void PutCollisionObject(CollisionComponent* collisionComponent);
		
//		static void RemoveCollisionObjectInObjectNode(GameObject gameObject);

	private:
		static vector<CollisionComponent*>* collisionList;
		vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>* collisionPairvector;
		map<string, GLint> collisionwithballmap;
		GLdouble ballCollideWithCorner = -1;
		
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
	void FreshTime() {
		timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
		prevTime = glutGet(GLUT_ELAPSED_TIME);;
	}

	void StartGame();
	Vector2 playerBoxBeforeReshape;
	GLdouble DeltaTime() {
		return timeSinceStart - prevTime;
	}

	GameManager(GameManager const&) = delete;
	void operator=(GameManager const&) = delete;

	GameObjectTree playerTree = GameObjectTree("player");
	Object* player;
	GameObjectTree tailTree = GameObjectTree("tail");
	Object* tail1;
	Object* tail2;
	Object* ear;
	void SetShakeTime();
	int shakeTime = 0;
	bool tail1upflag = true;
	bool tail1downflag = false;
	bool tail2upflag = false;
	bool tail2downflag = true;
	bool earleftflag = true;
	bool earrightflag = false;
	GameObjectTree enemyTree = GameObjectTree("enemyBox");
	Object* enemy;
	GameObjectTree netTree = GameObjectTree("net");
	Object* net;
	GameObjectTree wallTree = GameObjectTree("wall");
	
	GameObjectTree ballTree = GameObjectTree("ball");
	Object* ball;
	GameObjectTree electricity = GameObjectTree("electricity");

	static GLdouble BALL_VELOCITY;
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
	int enemyMoveTime = 0;
	const Vector2 BOXVELOCITYTORIGHT = Vector2(1, 0);
	const Vector2 BOXVELOCITYTOLEFT = Vector2(-1, 0);
	const Vector2 BOXVELOCITYZERO = Vector2(0, 0);

	/* need to set(dummy data)*/
	const Vector2 INITIAL_PLAYER_BOX_POSITION = Vector2(0, 0); //WORLDCOORDWINDOWWIDTH/16(100, 0)
	const Vector2 INITIAL_ENEMY_BOX_POSITION = Vector2((7* WORLDCOORDWINDOWWIDTH)/8, 0);
	const Vector2 INITIAL_BALL_POSITION = Vector2((15* WORLDCOORDWINDOWWIDTH)/32, (WORLDCOORDWINDOWHEIGHT*7)/10);
	const Vector2 INITIAL_NET_POSITION = Vector2((31 * WORLDCOORDWINDOWWIDTH) / 64, 0); //(31*WORLDCOORDWINDOWWIDTH)/64(775, 0)
	const Vector2 INITIAL_LEFT_WALL_POSITION = Vector2(-10, 0);
	const Vector2 INITIAL_RIGHT_WALL_POSITION = Vector2(WORLDCOORDWINDOWWIDTH, 0);
	const Vector2 INITIAL_TOP_WALL_POSITION = Vector2(0, WORLDCOORDWINDOWHEIGHT);

	bool playerBoxMoveRightFlag = false;
	bool playerBoxMoveLeftFlag = false;
	bool enemyBoxMoveRightFlag = false;
	bool enemyBoxMoveLeftFlag = false;
	bool ballLeftCollisionFlag = false;
	bool ballRightCollisionFlag = false;
	bool ballUpCollisionFlag = false;
	bool ballDownCollisionFlag = false;

	void InitializeGame();
	void InitBallVelocity();
	void InitObjectsPosition();
	void SetplayerBoxVelocity();
	void SetenemyBoxVelocity();
	void SetObjectPosition();
	void OneGameEnd(bool whoWin);
};