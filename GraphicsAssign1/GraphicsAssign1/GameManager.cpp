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

GLdouble GameManager::BALL_VELOCITY = 300;
GameManager::GameManager() {
	srand((unsigned int)time(0));
	CollisionComponent::ConnectCollisionManagerAddFunction(CollisionManager::PutCollisionObject);

	player = GetPlayer()->root->data->object;
	enemy = GetEnemy()->root->data->object;
	ball = GetBall()->root->data->object;

	objectsTreeVectorForDraw.push_back(*GetPlayer());
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
	SetplayerBoxVelocity();
	SetenemyBoxVelocity();
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
	InitBallVelocity();
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
게임 시작 시 공의 초기 속도를 결정해주는 함수
**/

void GameManager::InitBallVelocity() {
	BALL_VELOCITY = 300;
	int x = 0;
	int y = 0;
	
	while(0==x)
		x = rand()%101-50;

	while (-20 < y && y < 20)
		y = rand() % 101 - 50;

	ball->SetVelocity((Vector2::normalize(Vector2(x, y)))*BALL_VELOCITY);
}


void GameManager::SetplayerBoxVelocity() {
	if (playerBoxMoveRightFlag)
		player->SetVelocity(BOXVELOCITYTORIGHT);
	else if (playerBoxMoveLeftFlag)
		player->SetVelocity(BOXVELOCITYTOLEFT);
	else
		player->SetVelocity(BOXVELOCITYZERO);
	playerBoxMoveRightFlag = false;
	playerBoxMoveLeftFlag = false;
}

void GameManager::SetenemyBoxVelocity() {
	if (enemyMoveTime > 3000)
		enemyMoveTime = -5000;
	enemyMoveTime += DeltaTime();
	if (enemyMoveTime > 0)
	{
		if (ball->position.x + ball->width / 2 > enemy->position.x + enemy->width / 2)
			enemyBoxMoveRightFlag = true;
		else if (ball->position.x + ball->width / 2 < enemy->position.x + enemy->width / 2)
			enemyBoxMoveLeftFlag = true;
		if (enemyBoxMoveRightFlag)
			enemy->SetVelocity(BOXVELOCITYTORIGHT);
		else if (enemyBoxMoveLeftFlag)
			enemy->SetVelocity(BOXVELOCITYTOLEFT);
		else
			enemy->SetVelocity(BOXVELOCITYZERO);
		enemyBoxMoveRightFlag = false;
		enemyBoxMoveLeftFlag = false;
	}
	else
	{
		enemy->SetVelocity(BOXVELOCITYZERO);
		enemyBoxMoveRightFlag = false;
		enemyBoxMoveLeftFlag = false;
	}
}
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


