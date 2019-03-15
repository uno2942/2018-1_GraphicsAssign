#include "GameManager.h"
#include<ctime>
#include<cmath>
using namespace std;
GLdouble BALL_VELOCITY = 300;
/**
여기서 srand 함수를 실행시킨다.
**/
GameManager::GameManager() {
	srand((unsigned int)time(0));
	StartGame();
}

void GameManager::SavePlayerPositionBeforeReshape() {
	playerBoxBeforeReshape = playerBox.position;
}
void GameManager::LoadPlayerPositionBeforeReshape() {
	playerBox.position = playerBoxBeforeReshape;
}
void GameManager::OneFramePipeline() {
	timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	SetplayerBoxVelocity();
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

void GameManager::InitializeGame() {
	oneGameEnd = false;
	InitObjectsPosition();
	InitBallVelocity();
	timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	prevTime = glutGet(GLUT_ELAPSED_TIME);
}
void GameManager::StartGame() {
	myScore = 0;
	enemyScore = 0;
	WhoFinallyWin = 0;
	InitializeGame();
	/*...*/
}
/**
World coord. 기준
**/
void GameManager::InitObjectsPosition() {
	playerBox.SetPosition(INITIAL_PLAYER_BOX_POSITION);
	enemyBox.SetPosition(INITIAL_ENEMY_BOX_POSITION);
	ball.SetPosition(INITIAL_BALL_POSITION);
	net.SetPosition(INITIAL_NET_POSITION);
	leftwall.SetPosition(INITIAL_LEFT_WALL_POSITION);
	rightwall.SetPosition(INITIAL_RIGHT_WALL_POSITION);
	topwall.SetPosition(INITIAL_TOP_WALL_POSITION);
}

/**
게임 시작 시 공의 초기 속도를 결정해주는 함수
**/

void GameManager::InitBallVelocity() {
	int x = 0;
	int y = 0;
	
	while(0==x)
		x = rand()%101-50;

	while (0 == y)
		y = rand() % 101 - 50;

	x = 0;
	y = -10;
	ball.SetVelocity((Vector2::normalize(Vector2(x, y)))*BALL_VELOCITY);
}


void GameManager::SetplayerBoxVelocity() {
	if (playerBoxMoveRightFlag)
		playerBox.SetVelocity(BOXVELOCITYTORIGHT);
	else if (playerBoxMoveLeftFlag)
		playerBox.SetVelocity(BOXVELOCITYTOLEFT);
	else
		playerBox.SetVelocity(BOXVELOCITYZERO);
	playerBoxMoveRightFlag = false;
	playerBoxMoveLeftFlag = false;
}

void GameManager::SetObjectPosition() {
	playerBox.position += playerBox.velocity*PLAYER_BOX_VELOCITY;

	enemyBox.position += enemyBox.velocity*(((timeSinceStart - prevTime) / 1000.) + collisionManager.ballDeltaTime);
	ball.position += ball.velocity*((timeSinceStart - prevTime) / 1000.);
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


