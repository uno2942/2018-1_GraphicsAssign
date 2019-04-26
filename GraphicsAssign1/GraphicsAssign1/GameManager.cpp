#include<ctime>
#include "GameManager.h"
using namespace std;
/**
여기서 srand 함수를 실행시킨다.
**/
void GameManager::deleteTree(GameObjectTree& tree) {
	deleteTree_aux(tree.root);
}
void GameManager::deleteTree_aux(GameObjectNode* root) {
	if (root == NULL) return;
	GameObjectNode* temp1 = root->successor;
	GameObjectNode* temp2 = root->sibling;
	delete root;
	if (temp1 != NULL)
		deleteTree_aux(temp1);
	if (temp2 != NULL)
		deleteTree_aux(temp2);
}

GLint GameManager::BALL_VELOCITY = 300;
GameManager::GameManager() {
	srand((unsigned int)time(0));
	CollisionComponent::ConnectCollisionManagerAddFunction(CollisionManager::PutCollisionObject);

	player = GetPlayer()->root->data->object;
	enemy = GetEnemy()->root->data->object;
	ball = GetBall()->root->data->object;
	BALL_VELOCITY = 300;

	objectsTreeVectorForDraw.push_back(*(GetPlayer()));
	objectsTreeVectorForDraw.push_back(*GetEnemy());
	objectsTreeVectorForDraw.push_back(*GetBall());
	objectsTreeVectorForDraw.push_back(*GetLeftWall());
	objectsTreeVectorForDraw.push_back(*GetRightWall());
	objectsTreeVectorForDraw.push_back(*GetFrontWall());
	objectsTreeVectorForDraw.push_back(*GetBackWall());
	objectsTreeVectorForDraw.push_back(*GetBottomWall());
	StartGame();
	cout << "GameManager initiated" << endl;
}

GameManager::~GameManager() {
	deleteTree(*GetPlayer());
	deleteTree(*GetEnemy());
	deleteTree(*GetBall());
	objectsTreeVectorForDraw.push_back(*GetLeftWall());
	objectsTreeVectorForDraw.push_back(*GetRightWall());
	objectsTreeVectorForDraw.push_back(*GetFrontWall());
	objectsTreeVectorForDraw.push_back(*GetBackWall());
	objectsTreeVectorForDraw.push_back(*GetBottomWall());
}

void GameManager::OneFramePipeline() {
	timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	SetplayerBoxRotation(playerBoxRotateCounterClockwiseFlag, playerBoxRotateClockwiseFlag);
	SetplayerBoxVelocity(playerBoxMoveFrontFlag, playerBoxMoveBackFlag);
	playerBoxMoveFrontFlag = false;
	playerBoxMoveBackFlag = false;
	SetenemyBoxVelocity(enemyMoveTime += DeltaTime(), ball);
//	collisionManager.CollisionHandler(collisionManager.RestoreBallPosition(collisionManager.CollisionCheck()));
	SetObjectPosition();
	prevTime = timeSinceStart;
}

void GameManager::SpecialKeyboardInputHandler(GLint key) {
	switch (key) {
	case GLUT_KEY_RIGHT: playerBoxRotateCounterClockwiseFlag = true; break;
	case GLUT_KEY_LEFT: playerBoxRotateClockwiseFlag = true; break;
	case GLUT_KEY_UP: playerBoxMoveFrontFlag = true; break;
	case GLUT_KEY_DOWN: playerBoxMoveBackFlag = true; break;
	}
}

void GameManager::StartGame() {
	myScore = 0;
	enemyScore = 0;
	WhoFinallyWin = 0;
	InitializeGame();
}

void GameManager::InitializeGame() {
	oneGameEnd = false;
	enemyMoveTime = 0;
	InitObjectsPosition();
	InitBallVelocity(BALL_VELOCITY);
	timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	prevTime = glutGet(GLUT_ELAPSED_TIME);
}

void GameManager::InitObjectsPosition() {
	(*player).SetPosition(INITIAL_PLAYER_BOX_POSITION);
	(*enemy).SetPosition(INITIAL_ENEMY_BOX_POSITION);
	(*ball).SetPosition(INITIAL_BALL_POSITION);
}

/**
게임 시작 시 공의 초기 속도를 결정해주는 함수
**/



void GameManager::SetObjectPosition() {
	player->position += player->velocity*PLAYER_BOX_VELOCITY;
	enemy->position += enemy->velocity*ENEMY_BOX_VELOCITY;
	ball->position += ball->velocity*((((GLdouble)timeSinceStart - (GLdouble)prevTime) / 1000.) + collisionManager.ballDeltaTime);
	
}

void GameManager::OneGameEnd(bool whoWin) {
	if (whoWin)
		myScore++;
	else
		enemyScore++;
	oneGameEnd = true;
	if (myScore == THRESHOLDSCORE)
		WhoFinallyWin = 1;
	else if (enemyScore == THRESHOLDSCORE)
		WhoFinallyWin = 2;
	InitializeGame();
}
