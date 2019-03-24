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
typedef BinaryTree<Object*, string> ObjectTree;
typedef Node<Object*, string> ObjectNode;
class GameManager {
public:
	class SceneManager {

	};

	class CollisionManager {
	public:
		CollisionManager();
		vector<pair<pair<Object*, Object*>, Vector2>>* CollisionCheck();
		void CollisionHandler(vector<pair<pair<Object*, Object*>, Vector2>>* collisionPairvector);
		vector<pair<pair<Object*, Object*>, Vector2>>* RestoreBallPosition(vector<pair<pair<Object*, Object*>, Vector2>>* collisionPairvector);
		bool CheckCollisionAtUpSide(Object* o1, Object* o2, vector<pair<pair<Object*, Object*>, Vector2>>* collisionPairvector);
		bool CheckCollisionAtDownSide(Object* o1, Object* o2, vector<pair<pair<Object*, Object*>, Vector2>>* collisionPairvector);
		bool CheckCollisionAtLeftSide(Object* o1, Object* o2, vector<pair<pair<Object*, Object*>, Vector2>>* collisionPairvector);
		bool CheckCollisionAtRightSide(Object* o1, Object* o2, vector<pair<pair<Object*, Object*>, Vector2>>* collisionPairvector);
		void CheckCollision4side(Object* o1, Object* o2, vector<pair<pair<Object*, Object*>, Vector2>>* collisionPairvector);
		GLdouble ballDeltaTime = 0;
		void PutCollisionObject(ObjectTree* tree, Object object) {
			collisionList.push_back(CollisionObject(tree->root, object.shape, object.width, object.height, object.rotation));
		}
		void PutCollisionObject(ObjectNode* node, Object object) {
			collisionList.push_back(CollisionObject(node, object.shape, object.width, object.height, object.rotation));
		}
		void RemoveCollisionObject(ObjectTree* tree) {
			for (int i = 0; i < collisionList.size; i++)
				if (collisionList[i].objectNode == tree->root)
					collisionList.erase(collisionList.begin()+i);
		}
		void RemoveCollisionObject(ObjectNode* node) {
			for (int i = 0; i < collisionList.size; i++)
				if (collisionList[i].objectNode == node)
					collisionList.erase(collisionList.begin() + i);
		}
	private:
		class CollisionObject : public Object {
		public:
			ObjectNode* objectNode;
			CollisionObject(ObjectNode* _objectNode, Shape _shape, int _width, int _height, GLdouble _rotation = 0) : Object(_width, _height, _rotation) {
				objectNode = _objectNode;
				shape = _shape;
			}
		};
		vector<CollisionObject> collisionList;
		vector<pair<pair<Object*, Object*>, Vector2>>* collisionPairvector;
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
	Vector2 playerBoxBeforeReshape;
	GLdouble DeltaTime() {
		return timeSinceStart - prevTime;
	}
	GameManager(GameManager const&) = delete;
	void operator=(GameManager const&) = delete;

	ObjectTree playerTree = ObjectTree("player");
	Object* player;
	ObjectTree tailTree = ObjectTree("tail");
	ObjectTree earTree = ObjectTree("ear");

	ObjectTree enemyTree = ObjectTree("enemyBox");
	Object* enemy;
	ObjectTree netTree = ObjectTree("net");
	Object* net;
	ObjectTree wallTree = ObjectTree("wall");
	
	ObjectTree ballTree = ObjectTree("ball");
	Object* ball;
	ObjectTree electricity = ObjectTree("electricity");

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

	void StartGame();
	void InitializeGame();
	void InitBallVelocity();
	void InitObjectsPosition();
	void SetplayerBoxVelocity();
	void SetenemyBoxVelocity();
	void SetObjectPosition();
	void OneGameEnd(bool whoWin);
};