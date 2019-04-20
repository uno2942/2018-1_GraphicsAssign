#pragma once
#include<vector>
#include<map>
#include<utility>
#include"Objects.h"
#include"Units.h"
#include"CommonBetweenGameAndDraw.h"
#define THRESHOLDSCORE 13
#define ZBORDER_FOR_PLAYER 0
#define ZBORDER_FOR_ENEMY WORLD_COORD_MAP_ZLEN
using namespace std;

class GameManager {
public:
	class SceneManager {

	};

	class CollisionManager {
	public:
		CollisionManager();
		~CollisionManager();
		vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* CollisionCheck();
		void CollisionHandler(vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* collisionPairvector);
		vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* RestoreBallPosition(vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* collisionPairvector);
		bool CheckCollisionAtUpSide(const CollisionComponent& o1, const CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* collisionPairvector);
		bool CheckCollisionAtDownSide(const CollisionComponent& o1, const CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* collisionPairvector);
		bool CheckCollisionAtLeftSide(const CollisionComponent& o1, const CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* collisionPairvector);
		bool CheckCollisionAtRightSide(const CollisionComponent& o1, const CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* collisionPairvector);
		void CheckCollision4side(const CollisionComponent& o1, const CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* collisionPairvector);
		GLdouble ballDeltaTime = 0;

		static void PutCollisionObject(CollisionComponent* collisionComponent);
		
//		static void RemoveCollisionObjectInObjectNode(GameObject gameObject);

	private:
		static vector<CollisionComponent*>* collisionList;
		vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* collisionPairvector;
		map<string, GLint> collisionwithballmap;
		GLdouble ballCollideWithCorner = -1;
		
	};
	static GameManager& getInstance()
	{
		static GameManager instance;
		return instance;
	}

	void OneFramePipeline();

	void SpecialKeyboardInputHandler(int key);
	void FreshTime() {
		timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
		prevTime = glutGet(GLUT_ELAPSED_TIME);;
	}

	void StartGame();
	int DeltaTime() {
		return timeSinceStart - prevTime;
	}

	GameManager(GameManager const&) = delete;
	void operator=(GameManager const&) = delete;

	Object* player;
	Object* enemy;
	Object* ball;

	static GLdouble BALL_VELOCITY;
	int myScore = 0;
	int enemyScore = 0;
	int WhoFinallyWin = 0;

private:
	GameManager();
	~GameManager();
	void deleteTree(GameObjectTree& tree);
	void deleteTree_aux(GameObjectNode* root);
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
	GLdouble enemyMoveTime = 0;
	/* need to set(dummy data)*/
	
	bool playerBoxMoveFrontFlag = false;
	bool playerBoxMoveBackFlag = false;
	bool playerBoxRotateCounterClockwiseFlag = false;
	bool playerBoxRotateClockwiseFlag = false;
	
	bool ballLeftCollisionFlag = false;
	bool ballRightCollisionFlag = false;
	bool ballUpCollisionFlag = false;
	bool ballDownCollisionFlag = false;

	void InitializeGame();
	void InitObjectsPosition();
	void SetObjectPosition();
	void OneGameEnd(bool whoWin);
};