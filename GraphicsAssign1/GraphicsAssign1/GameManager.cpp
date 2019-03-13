#include "Objects.h"
#include "GameManager.h"
#include<ctime>
#include<cmath>
using namespace std;

/**
여기서 srand 함수를 실행시킨다.
**/
GameManager::GameManager() {
	srand((unsigned int)time(0));
}

void GameManager::SpecialKeyboardInputHandler(int key) {
	switch (key) {
	case GLUT_KEY_RIGHT: playerBoxMoveRightFlag = true; break;
	case GLUT_KEY_LEFT: playerBoxMoveLeftFlag = true;
	}
}

void GameManager::StartGame() {
	InitObjectPosition();
	InitBallVelocity();
	/*...*/
}
/**
World coord. 기준
**/
void GameManager::InitObjectPosition() {
	playerBox.SetPosition(InitialplayerBoxPosition);
	enemyBox.SetPosition(InitialenemyBoxPosition);
	ball.SetPosition(InitialBallPosition);
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

	double veclen = sqrt(x*x + y * y);
	ball.SetVelocity(x / veclen, y / veclen);
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

/**
현재는 Frame time을 고려하지 않고 짰습니다. 수정이 필요할 수 있습니다.
**/
void GameManager::SetObjectPosition() {
	playerBox.CurrentPosition() += playerBox.CurrentVelocity();
	enemyBox.CurrentPosition() += enemyBox.CurrentVelocity();
	ball.CurrentPosition() += ball.CurrentVelocity();
}
