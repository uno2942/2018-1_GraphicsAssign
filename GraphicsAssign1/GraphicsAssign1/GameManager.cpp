#include<ctime>
#include "GameManager.h"
using namespace std;
/**
���⼭ srand �Լ��� �����Ų��.
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

GLdouble GameManager::BALL_VELOCITY = 300;
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
	objectsTreeVectorForDraw.push_back(*GetWall());
	
	StartGame();
}

GameManager::~GameManager() {
	deleteTree(*GetPlayer());
	deleteTree(*GetEnemy());
	deleteTree(*GetBall());
	deleteTree(*GetWall());
}

void GameManager::OneFramePipeline() {
	timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	SetplayerBoxVelocity(playerBoxMoveRightFlag, playerBoxMoveLeftFlag);
	playerBoxMoveRightFlag = false;
	playerBoxMoveLeftFlag = false;
	SetenemyBoxVelocity(enemyMoveTime += DeltaTime(), ball);
	collisionManager.CollisionHandler(collisionManager.RestoreBallPosition(collisionManager.CollisionCheck()));
	SetObjectPosition();
	prevTime = timeSinceStart;
}

void GameManager::SpecialKeyboardInputHandler(int key) {
	switch (key) {
	case GLUT_KEY_RIGHT: playerBoxMoveRightFlag = true; break;
	case GLUT_KEY_LEFT: playerBoxMoveLeftFlag = true;
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
	GetWall()->root->data->object->SetPosition(INITIAL_LEFT_WALL_POSITION);
}

/**
���� ���� �� ���� �ʱ� �ӵ��� �������ִ� �Լ�
**/



void GameManager::SetObjectPosition() {
	player->position += player->velocity*PLAYER_BOX_VELOCITY;
	enemy->position += enemy->velocity*ENEMY_BOX_VELOCITY;
	ball->position += ball->velocity*(((timeSinceStart - prevTime) / 1000.) + collisionManager.ballDeltaTime);
	
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
