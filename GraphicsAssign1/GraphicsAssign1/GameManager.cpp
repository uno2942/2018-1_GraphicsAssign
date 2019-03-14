#include "GameManager.h"
#include<ctime>
#include<cmath>
using namespace std;

/**
���⼭ srand �Լ��� �����Ų��.
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
����� Frame time�� ������� �ʰ� ®���ϴ�. ������ �ʿ��� �� �ֽ��ϴ�.
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
������ó�� �� § �ڵ�� �Ŀ� ��ĥ �ʿ䰡 �ֽ��ϴ�.
-�� return�ϸ� �浹���� �ʾҴٴ� ���� �ǹ��մϴ�.
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
������ó�� �� § �ڵ�� �Ŀ� ��ĥ �ʿ䰡 �ֽ��ϴ�.
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
������ó�� �� § �ڵ�� �Ŀ� ��ĥ �ʿ䰡 �ֽ��ϴ�.
**/
void GameManager::CollisionManager::CheckCollisionAtUpSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList) {
	if (Object::Shape::CIRCLE == o1->shape && Object::Shape::BOX == o2->shape)
	{
		//collisionPairList->push_back(make_pair(make_pair(o1, o2), Vector2(0, 0)));
	}
}
/**
������ó�� �� § �ڵ�� �Ŀ� ��ĥ �ʿ䰡 �ֽ��ϴ�.
**/
void GameManager::CollisionManager::CheckCollisionAtDownSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList) {
	if (Object::Shape::BOX == o1->shape && Object::Shape::BOX == o2->shape)
	{
	}
}