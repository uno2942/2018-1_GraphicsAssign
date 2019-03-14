#include "GameManager.h"
#include<ctime>
#include<cmath>
using namespace std;

/**
여기서 srand 함수를 실행시킨다.
**/
GameManager::GameManager() {
	srand((unsigned int)time(0));
	StartGame();
}

void GameManager::OneFramePipeline() {
	SetplayerBoxVelocity();
	collisionManager.CollisionHandler(collisionManager.CollisionCheck());
	SetObjectPosition();
}
GameManager::CollisionManager::CollisionManager() {
	collisionPairList = new list<pair<pair<Object*, Object*>, Vector2>>();
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
	y = -1;
	x = -10;

	double veclen = sqrt(x*x + y * y);
	ball.SetVelocity( 10*(x / veclen), 10*(y / veclen));
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
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	cout << timeSinceStart << endl;
	playerBox.position += playerBox.velocity*(timeSinceStart/1000.);
	enemyBox.position += enemyBox.velocity*(timeSinceStart / 1000.);
	ball.position += ball.velocity*(timeSinceStart / 1000.);
	//cout << "Player: " << playerBox.position << '\t' << "Ball: " << ball.position << endl;
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


void GameManager::CollisionManager::CollisionHandler(list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList)
{
	while (collisionPairList != NULL && !(collisionPairList->empty())) {
		collisionPairList->back().first.first->velocity += collisionPairList->back().second;
		collisionPairList->pop_back();
	}
}



list<pair<pair<Object*, Object*>, Vector2>>* GameManager::CollisionManager::CollisionCheck() {
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
		CheckCollision4side(&(GameManager::getInstance().playerBox), &GameManager::getInstance().net, collisionPairList);
		CheckCollision4side(&GameManager::getInstance().playerBox, &GameManager::getInstance().screen, collisionPairList);
		CheckCollision4side(&GameManager::getInstance().ball, &GameManager::getInstance().playerBox, collisionPairList);
		CheckCollision4side(&GameManager::getInstance().enemyBox, &GameManager::getInstance().net, collisionPairList);
		CheckCollision4side(&GameManager::getInstance().enemyBox, &GameManager::getInstance().screen, collisionPairList);
		CheckCollision4side(&GameManager::getInstance().ball, &GameManager::getInstance().enemyBox, collisionPairList);
		CheckCollision4side(&GameManager::getInstance().ball, &GameManager::getInstance().screen, collisionPairList);
		return collisionPairList;
	}
}


void GameManager::CollisionManager::CheckCollision4side(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList) {
	CheckCollisionAtRightSide(o1, o2, collisionPairList);
	CheckCollisionAtLeftSide(o1, o2, collisionPairList);
	CheckCollisionAtDownSide(o1, o2, collisionPairList);
	CheckCollisionAtUpSide(o1, o2, collisionPairList);
}

/**
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
-를 return하면 충돌하지 않았다는 것을 의미합니다.
**/
void GameManager::CollisionManager::CheckCollisionAtRightSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList) {

	if (Object::Shape::BOX == o1->shape && Object::Shape::BOX == o2->shape)
	{
		if (o2->name == "screen")
		{
			if (o1->name == "enemy")
			{
				if (o1->position.x + o1->width > o2->position.x + o2->width)
					collisionPairList->push_back(make_pair(make_pair(o1, o2), Vector2(-1, 0)));
			}
			else if (o1->name == "ball")
			{
				if (o1->position.x + o1->width > o2->position.x+ o2->width)
					collisionPairList->push_back(make_pair(make_pair(o1, o2), Vector2(-2*(o1->velocity.x), 0)));
			}
		}
		//else if (o1->position.x < o2->position.x && o1->position.x + o1->width >= o2->position.x)
		//	collisionPairList->push_back(make_pair(make_pair(o1, o2), Vector2(-1, 0)));
	}

}
/**
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
**/
void GameManager::CollisionManager::CheckCollisionAtLeftSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList) {
	if (Object::Shape::BOX == o1->shape && Object::Shape::BOX == o2->shape)
	{
		if (o2->name == "screen")
		{
			if (o1->name == "player")
			{
				if (o1->position.x < o2->position.x)
					collisionPairList->push_back(make_pair(make_pair(o1, o2), Vector2(-1, 0)));
			}
			else if (o1->name == "ball")
			{
				if (o1->position.x < o2->position.x)
					collisionPairList->push_back(make_pair(make_pair(o1, o2), Vector2(-2*(o1->velocity.x), 0)));
			}
		}
		//else if (o1->position.x + o1->width > o2->position.x + o2->width && o1->position.x <= o2->position.x + o2->width)
		//	collisionPairList->push_back(make_pair(make_pair(o1, o2), Vector2(1, 0)));
	}
}
/**
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
**/
void GameManager::CollisionManager::CheckCollisionAtUpSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList) {
	if (Object::Shape::CIRCLE == o1->shape && Object::Shape::BOX == o2->shape)
	{
		//collisionPairList->push_back(make_pair(make_pair(o1, o2), Vector2(0, 0)));
	}
}
/**
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
**/
void GameManager::CollisionManager::CheckCollisionAtDownSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList) {
	if (Object::Shape::BOX == o1->shape && Object::Shape::BOX == o2->shape)
	{
	}
}