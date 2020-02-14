#include<ctime>
#include "GameManager.h"
using namespace std;
using namespace glm;
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
	delete root->data;
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
	leftWall = GetLeftWall()->root->data->object;
	rightWall = GetRightWall()->root->data->object;
	frontWall = GetFrontWall()->root->data->object;
	backWall = GetBackWall()->root->data->object;
	bottomWall = GetBottomWall()->root->data->object;
	mLight = new myLight();
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
	deleteTree(*GetLeftWall());
	deleteTree(*GetRightWall());
	deleteTree(*GetFrontWall());
	deleteTree(*GetBackWall());
	deleteTree(*GetBottomWall());
	if(mLight != NULL)
		delete mLight;
	deleteindraw();
}

void GameManager::OneFramePipeline() {
	timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	SetplayerBoxRotation(playerBoxRotateCounterClockwiseFlag, playerBoxRotateClockwiseFlag);
	SetplayerBoxVelocity(playerBoxMoveFrontFlag, playerBoxMoveBackFlag);
	playerBoxMoveFrontFlag = false;
	playerBoxMoveBackFlag = false;
	playerBoxRotateCounterClockwiseFlag = false;
	playerBoxRotateClockwiseFlag = false;
	enemyMoveTime += DeltaTime();
	SetenemyBoxVelocity(&enemyMoveTime, ball);
	collisionManager.CollisionHandler(collisionManager.RestoreBallPosition(collisionManager.CollisionCheck()));
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
	mLightTheta = 0;
	isSun = true;
	mLight->SetDirectionalLightI(0.125, 0.5);
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
	player->SetPosition(INITIAL_PLAYER_BOX_POSITION);
	player->SetRotation(3.141592);
	player->rotationAxis = Vector3(0, 0, 1);

	enemy->SetPosition(INITIAL_ENEMY_BOX_POSITION);
	enemy->rotationAxis = Vector3(0, 0, 1);
	enemy->SetRotation(0);

	ball->SetPosition(INITIAL_BALL_POSITION);	
	ball->rotationAxis = Vector3(0, 0, 1);
	ball->SetRotation(0);

	mLight->SetLightPosition(vec3(INITIAL_BALL_POSITION.x, INITIAL_BALL_POSITION.y + 300, INITIAL_BALL_POSITION.z));
	mLight->DirectionalLightDirection(vec3(cos(mLightTheta), sin(mLightTheta), 0));
}

/**
게임 시작 시 공의 초기 속도를 결정해주는 함수
**/



void GameManager::SetObjectPosition() {
	player->position += player->velocity*PLAYER_BOX_VELOCITY;
	enemy->position += enemy->velocity*ENEMY_BOX_VELOCITY;
	ball->position += ball->velocity*((((GLdouble)timeSinceStart - (GLdouble)prevTime) / 1000.) + collisionManager.ballDeltaTime);
	cout << timeSinceStart << ' ' << prevTime << endl;
	mLight->SetLightPosition(vec3(ball->position.x, ball->position.y+300, ball->position.z));
	mLightTheta += 0.001;
	if (mLightTheta > 3.141592)
	{
		if (isSun)
		{
			isSun = false;
			mLightTheta -= 3.141592;
			mLight->SetDirectionalLightI(0.125/4, 0.5/4);
		}
		else
		{
			isSun = true;
			mLightTheta -= 3.141592;;
			mLight->SetDirectionalLightI(0.125, 0.5);
		}
	}
	mLight->DirectionalLightDirection(vec3(cos(mLightTheta), sin(mLightTheta), 0));
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
