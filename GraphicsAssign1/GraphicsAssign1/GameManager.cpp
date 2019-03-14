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

	x = 1;
	y = -10;
	double veclen = sqrt(x*x + y * y);
	ball.SetVelocity( 300*(x / veclen), 300*(y / veclen));
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
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	playerBox.position += playerBox.velocity;

	enemyBox.position += enemyBox.velocity*((timeSinceStart - prevTime) / 1000.);
	ball.position += ball.velocity*((timeSinceStart - prevTime) / 1000.);
	prevTime = timeSinceStart;
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


//공의 경우 vector normalization 체크
void GameManager::CollisionManager::CollisionHandler(list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList)
{
	bool doubleCollision = (!GameManager::getInstance().ballUpCollisionFlag || GameManager::getInstance().ballDownCollisionFlag || GameManager::getInstance().ballRightCollisionFlag || GameManager::getInstance().ballLeftCollisionFlag) &&

		(GameManager::getInstance().ballUpCollisionFlag || !GameManager::getInstance().ballDownCollisionFlag || GameManager::getInstance().ballRightCollisionFlag || GameManager::getInstance().ballLeftCollisionFlag) &&

		(GameManager::getInstance().ballUpCollisionFlag || GameManager::getInstance().ballDownCollisionFlag || !GameManager::getInstance().ballRightCollisionFlag && GameManager::getInstance().ballLeftCollisionFlag) &&

		(GameManager::getInstance().ballUpCollisionFlag || GameManager::getInstance().ballDownCollisionFlag || GameManager::getInstance().ballRightCollisionFlag || !GameManager::getInstance().ballLeftCollisionFlag);


	while (collisionPairList != NULL && !(collisionPairList->empty())) {
		if (!doubleCollision && collisionPairList->back().first.first->shape == Object::Shape::CIRCLE)
		{
			if (
				(collisionPairList->back().second.x == 0 && collisionPairList->back().second.y > 0 && GameManager::getInstance().ballUpCollisionFlag) ||
				(collisionPairList->back().second.x == 0 && collisionPairList->back().second.y < 0 && GameManager::getInstance().ballDownCollisionFlag) ||
				(collisionPairList->back().second.x > 0 && collisionPairList->back().second.y == 0 && GameManager::getInstance().ballRightCollisionFlag) ||
				(collisionPairList->back().second.x < 0 && collisionPairList->back().second.y == 0 && GameManager::getInstance().ballLeftCollisionFlag)
				)
			{
				collisionPairList->pop_back();
				continue;
			}
		}
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
		CheckCollisionAtRightSide(&(GameManager::getInstance().playerBox), &GameManager::getInstance().net, collisionPairList);
		CheckCollisionAtLeftSide(&GameManager::getInstance().playerBox, &GameManager::getInstance().leftwall, collisionPairList);
		
		CheckCollisionAtLeftSide(&GameManager::getInstance().enemyBox, &GameManager::getInstance().net, collisionPairList);
		CheckCollisionAtRightSide(&GameManager::getInstance().enemyBox, &GameManager::getInstance().rightwall, collisionPairList);

		CheckCollision4side(&GameManager::getInstance().ball, &GameManager::getInstance().playerBox, collisionPairList);
		CheckCollision4side(&GameManager::getInstance().ball, &GameManager::getInstance().enemyBox, collisionPairList);
		CheckCollision4side(&GameManager::getInstance().ball, &GameManager::getInstance().net, collisionPairList);
		CheckCollisionAtLeftSide(&GameManager::getInstance().ball, &GameManager::getInstance().leftwall, collisionPairList);
		CheckCollisionAtRightSide(&GameManager::getInstance().ball, &GameManager::getInstance().rightwall, collisionPairList);
		CheckCollisionAtUpSide(&GameManager::getInstance().ball, &GameManager::getInstance().topwall, collisionPairList);
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
o1이 o2를 오른쪽에서 충돌
**/
void GameManager::CollisionManager::CheckCollisionAtRightSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList) {

	if (Object::Shape::BOX == o1->shape && Object::Shape::BOX == o2->shape)
	{
		if (o1->position.x < o2->position.x && o1->position.x + o1->width >= o2->position.x)
			collisionPairList->push_back(make_pair(make_pair(o1, o2), Vector2(-1, 0)));
	}
	else if (Object::Shape::CIRCLE == o1->shape && Object::Shape::BOX == o2->shape) {
		if (o1->position.x < o2->position.x && o1->position.x + o1->width >= o2->position.x &&
			(o1->position.y + (o1->height / 2) >= o2->position.y && o1->position.y + (o1->height / 2) <= o2->position.y + o2->height))
		{
			collisionPairList->push_back(make_pair(make_pair(o1, o2), Vector2(-2 * (o1->velocity.x), 0)));
			GameManager::getInstance().ballRightCollisionFlag = true;
		}
		else
			GameManager::getInstance().ballRightCollisionFlag = false;
	}
}
/**
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
o1이 o2를 왼쪽에서 충돌
**/
void GameManager::CollisionManager::CheckCollisionAtLeftSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList) {
	if (Object::Shape::BOX == o1->shape && Object::Shape::BOX == o2->shape)
	{
		if (o1->position.x + o1->width > o2->position.x + o2->width && o1->position.x <= o2->position.x + o2->width)
			collisionPairList->push_back(make_pair(make_pair(o1, o2), Vector2(1, 0)));
	}
	else if (Object::Shape::CIRCLE == o1->shape && Object::Shape::BOX == o2->shape) {
		if (o1->position.x + o1->width > o2->position.x + o2->width && o1->position.x <= o2->position.x + o2->width &&
			(o1->position.y + (o1->height / 2) >= o2->position.y && o1->position.y + (o1->height / 2) <= o2->position.y + o2->height))
		{
			collisionPairList->push_back(make_pair(make_pair(o1, o2), Vector2(-2 * (o1->velocity.x), 0)));
			GameManager::getInstance().ballLeftCollisionFlag = true;
		}
		else
			GameManager::getInstance().ballLeftCollisionFlag = false;
	}
}



/**
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
**/
void GameManager::CollisionManager::CheckCollisionAtUpSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList) {
	if (Object::Shape::CIRCLE == o1->shape && Object::Shape::BOX == o2->shape)
	{
		if (o1->position.y < o2->position.y && o1->position.y + o1->height >= o2->position.y)
		{
			collisionPairList->push_back(make_pair(make_pair(o1, o2), Vector2(0, -2 * (o1->velocity.y))));
			GameManager::getInstance().ballUpCollisionFlag = true;
		}
		else
			GameManager::getInstance().ballUpCollisionFlag = false;
	}
}



/**
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
**/
void GameManager::CollisionManager::CheckCollisionAtDownSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList) {
	if (Object::Shape::CIRCLE == o1->shape && Object::Shape::BOX == o2->shape)
	{
		if (o1->position.y < o2->position.y + o2->height && o1->position.y + o1->height >= o2->position.y + o2->height &&
			(o1->position.x + (o1->width / 2) >= o2->position.x && o1->position.x + (o1->width / 2) <= o2->position.x + o2->width))
		{
			collisionPairList->push_back(make_pair(make_pair(o1, o2), Vector2(0, -2 * (o1->velocity.y))));
			GameManager::getInstance().ballDownCollisionFlag = true;
		}
		else
			GameManager::getInstance().ballDownCollisionFlag = false;
	}
}