#include "GameManager.h"
#include<ctime>
#include<cmath>
#include<freeglut.h>
using namespace std;

/**
여기서 srand 함수를 실행시킨다.
**/
GameManager::GameManager() {
	srand((unsigned int)time(0));
	ball.width = 100;
	ball.height = 100;
	net.width = 50;
	net.height = 500;
	playerBox.width = 150;
	playerBox.height = 50;
	enemyBox.width = 150;
	enemyBox.height = 50;
	screen.width = WORLDCOORDWINDOWWIDTH;
	screen.height = WORLDCOORDWINDOWHEIGHT;
	StartGame();
}

void GameManager::OneFramePipeline() {
	SetplayerBoxVelocity();
	collisionManager.CollisionHandler(collisionManager.CollisionCheck());
	SetObjectPosition();
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
	oneGameEnd = false;
	wholeGameEnd = false;
	WhoFinallyWin = 0;
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
	ball.SetVelocity( x / veclen, y / veclen);
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
}


void GameManager::CollisionManager::CollisionHandler(list<pair<pair<Object, Object>, Vector2>>* collisionPairList)
{
	while (collisionPairList !=NULL && collisionPairList->empty()) {
		collisionPairList->back().first.first.velocity += collisionPairList->back().second;
		collisionPairList->pop_back();
	}
	delete collisionPairList;
}



list<pair<pair<Object, Object>, Vector2>>* GameManager::CollisionManager::CollisionCheck() {
	list<pair<pair<Object, Object>, Vector2>>* collisionPairList = new list<pair<pair<Object, Object>, Vector2>>();
	if (GameManager::getInstance().ball.position.y <= GameManager::getInstance().screen.position.y)
	{
		if (GameManager::getInstance().ball.position.x + (GameManager::getInstance().ball.width / 2) <= GameManager::getInstance().net.position.x)
		{
			GameManager::getInstance().OneGameEnd(false);
			return collisionPairList;
		}
		else if (GameManager::getInstance().ball.position.x + (GameManager::getInstance().ball.GetSize().x / 2) >= GameManager::getInstance().net.position.x + GameManager::getInstance().net.width)
		{
			GameManager::getInstance().OneGameEnd(true);
			return collisionPairList;
		}
		else
			throw;
	}
	else
	{
		CheckCollision4side(GameManager::getInstance().playerBox, GameManager::getInstance().net, collisionPairList);
		CheckCollision4side(GameManager::getInstance().playerBox, GameManager::getInstance().screen, collisionPairList);
		CheckCollision4side(GameManager::getInstance().ball, GameManager::getInstance().playerBox, collisionPairList);
		CheckCollision4side(GameManager::getInstance().enemyBox, GameManager::getInstance().net, collisionPairList);
		CheckCollision4side(GameManager::getInstance().enemyBox, GameManager::getInstance().screen, collisionPairList);
		CheckCollision4side(GameManager::getInstance().ball, GameManager::getInstance().enemyBox, collisionPairList);
		CheckCollision4side(GameManager::getInstance().ball, GameManager::getInstance().screen, collisionPairList);
		return collisionPairList;
	}
}


void GameManager::CollisionManager::CheckCollision4side(const Object& o1, const Object& o2, list<pair<pair<Object, Object>, Vector2>>* collisionPairList) {
	CheckCollisionAtRightSide(o1, o2, collisionPairList);
	CheckCollisionAtLeftSide(o1, o2, collisionPairList);
	CheckCollisionAtDownSide(o1, o2, collisionPairList);
	CheckCollisionAtUpSide(o1, o2, collisionPairList);
}

/**
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
-를 return하면 충돌하지 않았다는 것을 의미합니다.
**/
void GameManager::CollisionManager::CheckCollisionAtRightSide(const Object& o1, const Object& o2, list<pair<pair<Object, Object>, Vector2>>* collisionPairList) {
	if (Object::Shape::BOX == o1.shape && Object::Shape::BOX == o2.shape)
	{
		if (o1.position.x < o2.position.x && o1.position.x + o1.width >= o2.position.x)
			collisionPairList->push_back(make_pair(make_pair(o1, o2), Vector2(-o1.velocity.x, 0)));
	}
}
/**
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
**/
void GameManager::CollisionManager::CheckCollisionAtLeftSide(const Object& o1, const Object& o2, list<pair<pair<Object, Object>, Vector2>>* collisionPairList) {
	if (Object::Shape::BOX == o1.shape && Object::Shape::BOX == o2.shape)
	{
		if (o1.position.x + o1.width > o2.position.x + o2.width && o1.position.x <= o2.position.x + o2.width)
			collisionPairList->push_back(make_pair(make_pair(o1, o2), Vector2(o1.velocity.x, 0)));
	}
}
/**
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
**/
void GameManager::CollisionManager::CheckCollisionAtUpSide(const Object& o1, const Object& o2, list<pair<pair<Object, Object>, Vector2>>* collisionPairList) {
	if (Object::Shape::CIRCLE == o1.shape && Object::Shape::BOX == o2.shape)
	{
		//collisionPairList->push_back(make_pair(make_pair(o1, o2), Vector2(0, 0)));
	}
}
/**
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
**/
void GameManager::CollisionManager::CheckCollisionAtDownSide(const Object& o1, const Object& o2, list<pair<pair<Object, Object>, Vector2>>* collisionPairList) {
	if (Object::Shape::BOX == o1.shape && Object::Shape::BOX == o2.shape)
	{
	}
}