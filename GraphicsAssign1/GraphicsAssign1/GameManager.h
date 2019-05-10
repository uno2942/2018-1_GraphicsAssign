#pragma once
#include<vector>
#include<map>
#include<utility>
#include"Objects.h"
#include"Units.h"
#include"CommonBetweenGameAndDraw.h"
#include"Light.h"
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
		bool CheckCollisionAtUpSide(CollisionComponent& o1, CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* collisionPairvector);
		bool CheckCollisionAtDownSide(CollisionComponent& o1, CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* collisionPairvector);
		bool CheckCollisionAtLeftSide(CollisionComponent& o1, CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* collisionPairvector);
		bool CheckCollisionAtRightSide(CollisionComponent& o1, CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* collisionPairvector);
		void CheckCollision4side(CollisionComponent& o1, CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* collisionPairvector);
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

	void SpecialKeyboardInputHandler(GLint key);
	void FreshTime() {
		timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
		prevTime = glutGet(GLUT_ELAPSED_TIME);;
	}

	void StartGame();
	GLint DeltaTime() {
		return timeSinceStart - prevTime;
	}

	GameManager(GameManager const&) = delete;
	void operator=(GameManager const&) = delete;

	Object* player;
	Object* enemy;
	Object* ball;

	Object* leftWall;
	Object* rightWall;
	Object* frontWall;
	Object* backWall;
	Object* bottomWall;

	myLight* mLight = NULL;

	static GLint BALL_VELOCITY;
	GLint myScore = 0;
	GLint enemyScore = 0;
	GLint WhoFinallyWin = 0;

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
	
	GLint timeSinceStart = 0;
	GLint prevTime = 0;
	GLint enemyMoveTime = 0;
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