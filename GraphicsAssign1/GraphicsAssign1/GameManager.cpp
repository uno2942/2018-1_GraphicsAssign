#include "Objects.h"
#include "GameManager.h"
#include<ctime>
#include<cmath>
using namespace std;

/**
���⼭ srand �Լ��� �����Ų��.
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
World coord. ����
**/
void GameManager::InitObjectPosition() {
	playerBox.SetPosition(InitialplayerBoxPosition);
	enemyBox.SetPosition(InitialenemyBoxPosition);
	ball.SetPosition(InitialBallPosition);
}

/**
���� ���� �� ���� �ʱ� �ӵ��� �������ִ� �Լ�
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
����� Frame time�� ������� �ʰ� ®���ϴ�. ������ �ʿ��� �� �ֽ��ϴ�.
**/
void GameManager::SetObjectPosition() {
	playerBox.CurrentPosition() += playerBox.CurrentVelocity();
	enemyBox.CurrentPosition() += enemyBox.CurrentVelocity();
	ball.CurrentPosition() += ball.CurrentVelocity();
}
