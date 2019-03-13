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
	InitObjectsPosition();
	InitBallVelocity();
	/*...*/
}
/**
World coord. ����
**/
void GameManager::InitObjectsPosition() {
	playerBox.SetPosition(INITIALPLAYERBOXPOSITION);
	enemyBox.SetPosition(INITIALENEMYBOXPOSITION);
	ball.SetPosition(INITIALBALLPOSITION);
	net.SetPosition(INITIALNETPOSITION);
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
	playerBox.position += playerBox.GetCurrentVelocity();
	enemyBox.position += enemyBox.GetCurrentVelocity();
	ball.position += ball.GetCurrentVelocity();
}

void GameManager::GameEnd(bool whoWin) {
	if (whoWin)
		myScore++;
	else
		enemyScore++;
	gameEnd = true;
}
list<pair<pair<Object, Object>, Vector2>>* GameManager::CollisionManager::CollisionCheck() {
	list<pair<pair<Object, Object>, Vector2>>* collisionPairList = new list<pair<pair<Object, Object>, Vector2>>();
	if (GameManager::getInstance().ball.GetCurrentPosition().y <= GameManager::getInstance().screen.GetCurrentPosition().y)
	{
		if (GameManager::getInstance().ball.GetCurrentPosition().x + (GameManager::getInstance().ball.GetSize().x / 2) <= GameManager::getInstance().net.GetCurrentPosition().x)
		{
			GameManager::getInstance().GameEnd(false);
			return NULL;
		}
		else if (GameManager::getInstance().ball.GetCurrentPosition().x + (GameManager::getInstance().ball.GetSize().x / 2) >= GameManager::getInstance().net.GetCurrentPosition().x + GameManager::getInstance().net.GetSize().x)
		{
			GameManager::getInstance().GameEnd(true);
			return NULL;
		}
		else
			throw;
	}
}

/**
������ó�� �� § �ڵ�� �Ŀ� ��ĥ �ʿ䰡 �ֽ��ϴ�.
-�� return�ϸ� �浹���� �ʾҴٴ� ���� �ǹ��մϴ�.
**/
Vector2 GameManager::CollisionManager::CheckCollisionAtRightSide(const Object& o1, const Object& o2) {
	if (Object::Shape::BOX == o1.shape && Object::Shape::BOX == o2.shape)
	{
		if (o1.position.x + o1.width >= o2.position.x)
			return Vector2(o2.position.x, 0);
		return Vector2(-10, -10);
	}
}
/**
������ó�� �� § �ڵ�� �Ŀ� ��ĥ �ʿ䰡 �ֽ��ϴ�.
**/
Vector2 GameManager::CollisionManager::CheckCollisionAtLeftSide(const Object& o1, const Object& o2) {
	if (Object::Shape::BOX == o1.shape && Object::Shape::BOX == o2.shape)
	{
		if (o1.position.x <= o2.position.x + o2.width)
			return Vector2(o1.position.x, 0);
		return Vector2(-10, -10);
	}
}
/**
������ó�� �� § �ڵ�� �Ŀ� ��ĥ �ʿ䰡 �ֽ��ϴ�.
**/
Vector2 GameManager::CollisionManager::CheckCollisionAtUpSide(const Object& o1, const Object& o2) {
	if (Object::Shape::BOX == o1.shape && Object::Shape::BOX == o2.shape)
	{
		return Vector2(-10, -10);
	}
}
/**
������ó�� �� § �ڵ�� �Ŀ� ��ĥ �ʿ䰡 �ֽ��ϴ�.
**/
Vector2 GameManager::CollisionManager::CheckCollisionAtDownSide(const Object& o1, const Object& o2) {
	if (Object::Shape::BOX == o1.shape && Object::Shape::BOX == o2.shape)
	{
		return Vector2(-10, -10);
	}
}