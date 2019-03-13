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
	InitObjectsPosition();
	InitBallVelocity();
	/*...*/
}
/**
World coord. 기준
**/
void GameManager::InitObjectsPosition() {
	playerBox.SetPosition(INITIALPLAYERBOXPOSITION);
	enemyBox.SetPosition(INITIALENEMYBOXPOSITION);
	ball.SetPosition(INITIALBALLPOSITION);
	net.SetPosition(INITIALNETPOSITION);
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
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
-를 return하면 충돌하지 않았다는 것을 의미합니다.
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
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
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
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
**/
Vector2 GameManager::CollisionManager::CheckCollisionAtUpSide(const Object& o1, const Object& o2) {
	if (Object::Shape::BOX == o1.shape && Object::Shape::BOX == o2.shape)
	{
		return Vector2(-10, -10);
	}
}
/**
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
**/
Vector2 GameManager::CollisionManager::CheckCollisionAtDownSide(const Object& o1, const Object& o2) {
	if (Object::Shape::BOX == o1.shape && Object::Shape::BOX == o2.shape)
	{
		return Vector2(-10, -10);
	}
}