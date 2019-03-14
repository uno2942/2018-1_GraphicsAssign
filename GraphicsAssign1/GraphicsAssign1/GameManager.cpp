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
	timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
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
	InitializeGame();
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
				(collisionPairList->back().second.x > 0 && GameManager::getInstance().ballRightCollisionFlag) ||
				(collisionPairList->back().second.x < 0 && GameManager::getInstance().ballLeftCollisionFlag)
				)
			{
				collisionPairList->pop_back();
				continue;
			}
		}
		collisionPairList->back().first.first->velocity += collisionPairList->back().second;
		if (collisionPairList->back().first.first->shape == Object::Shape::CIRCLE)
			cout << "Ball Velocity: " << Vector2::abs(collisionPairList->back().first.first->velocity) << endl;
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
	if (!CheckCollisionAtRightSide(o1, o2, collisionPairList))
	{
		if (!CheckCollisionAtLeftSide(o1, o2, collisionPairList))
		{
			if (!CheckCollisionAtDownSide(o1, o2, collisionPairList))
			{
				if (!CheckCollisionAtUpSide(o1, o2, collisionPairList))
				{

				}
			}
		}
	}
}

/**
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
o1이 o2를 오른쪽에서 충돌
**/
bool GameManager::CollisionManager::CheckCollisionAtRightSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList) {

	if (Object::Shape::BOX == o1->shape && Object::Shape::BOX == o2->shape)
	{
		if (o1->position.x < o2->position.x && o1->position.x + o1->width >= o2->position.x)
		{
			collisionPairList->push_back(make_pair(make_pair(o1, o2), Vector2(-1, 0)));
			return true;
		}
		return false;
	}
	else if (Object::Shape::CIRCLE == o1->shape && Object::Shape::BOX == o2->shape) {
		if (o1->position.x < o2->position.x && o1->position.x + o1->width >= o2->position.x &&
			(o1->position.y + (o1->height / 2) >= o2->position.y && o1->position.y + (o1->height / 2) <= o2->position.y + o2->height))
		{
			collisionPairList->push_back(make_pair(make_pair(o1, o2), Vector2(-2 * (o1->velocity.x), 0)));
			GameManager::getInstance().ballRightCollisionFlag = true;
		}
		else {
			Vector2 center = Vector2(o1->position.x + o1->width / 2, o1->position.y + o1->height / 2);
			Vector2 leftcorner = Vector2(o2->position.x, o2->position.y + o2->height);
			GLdouble r = o1->width / 2;
			Vector2 centertocorner = Vector2(center - leftcorner);
			GLdouble distance = Vector2::abs(centertocorner);
			if (r >= distance)
			{
				collisionPairList->push_back(make_pair(make_pair(o1, o2), 
					Vector2::normalize(centertocorner)*
					(2*BALL_VELOCITY*abs(o1->velocity.x*centertocorner.x+ o1->velocity.y*centertocorner.y)/
					(Vector2::abs(o1->velocity)*Vector2::abs(centertocorner)) )));
				cout << "Right" << Vector2::normalize(centertocorner)*
					(2 * BALL_VELOCITY*abs(o1->velocity.x*centertocorner.x + o1->velocity.y*centertocorner.y) /
					(Vector2::abs(o1->velocity)*distance)) << "cos len: " << abs(o1->velocity.x*centertocorner.x + o1->velocity.y*centertocorner.y) /
						(Vector2::abs(o1->velocity)*distance)<<endl;
				GameManager::getInstance().ballRightCollisionFlag = true;
			}
			else
				GameManager::getInstance().ballRightCollisionFlag = false;
		}
	}
	else {

	}
	return GameManager::getInstance().ballRightCollisionFlag;
}

/**
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
o1이 o2를 왼쪽에서 충돌
**/
bool GameManager::CollisionManager::CheckCollisionAtLeftSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList) {
	if (Object::Shape::BOX == o1->shape && Object::Shape::BOX == o2->shape)
	{
		if (o1->position.x + o1->width > o2->position.x + o2->width && o1->position.x <= o2->position.x + o2->width)
		{
			collisionPairList->push_back(make_pair(make_pair(o1, o2), Vector2(1, 0)));
			return true;
		}
		return false;
	}
	else if (Object::Shape::CIRCLE == o1->shape && Object::Shape::BOX == o2->shape) {
		if (o1->position.x + o1->width > o2->position.x + o2->width && o1->position.x <= o2->position.x + o2->width &&
			(o1->position.y + (o1->height / 2) >= o2->position.y && o1->position.y + (o1->height / 2) <= o2->position.y + o2->height))
		{
			collisionPairList->push_back(make_pair(make_pair(o1, o2), Vector2(-2 * (o1->velocity.x), 0)));
			GameManager::getInstance().ballLeftCollisionFlag = true;
		}
		else {
			Vector2 center = Vector2(o1->position.x + o1->width / 2, o1->position.y + o1->height / 2);
			Vector2 rightcorner = Vector2(o2->position.x+o2->width, o2->position.y + o2->height);
			GLdouble r = o1->width / 2;
			Vector2 centertocorner = Vector2(center - rightcorner);
			GLdouble distance = Vector2::abs(centertocorner);
			if (r >= distance)
			{
				collisionPairList->push_back(make_pair(make_pair(o1, o2),
					Vector2::normalize(centertocorner)*
					(2 * BALL_VELOCITY*abs(o1->velocity.x*centertocorner.x + o1->velocity.y*centertocorner.y) /
					(Vector2::abs(o1->velocity)*distance))));
				cout << "Left" << Vector2::normalize(centertocorner)*
					(2 * BALL_VELOCITY*abs(o1->velocity.x*centertocorner.x + o1->velocity.y*centertocorner.y) /
					(Vector2::abs(o1->velocity)*distance)) << "cos len: " << abs(o1->velocity.x*centertocorner.x + o1->velocity.y*centertocorner.y) /
						(Vector2::abs(o1->velocity)*distance) << endl;
				GameManager::getInstance().ballLeftCollisionFlag = true;
			}
			else
				GameManager::getInstance().ballLeftCollisionFlag = false;
		}
	}
	else
	{

	}
	return GameManager::getInstance().ballLeftCollisionFlag;
}



/**
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
**/
bool GameManager::CollisionManager::CheckCollisionAtUpSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList) {
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
	else
	{
		return true;
	}
	return GameManager::getInstance().ballUpCollisionFlag;
}



/**
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
**/
bool GameManager::CollisionManager::CheckCollisionAtDownSide(Object* o1, Object* o2, list<pair<pair<Object*, Object*>, Vector2>>* collisionPairList) {
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
	else {
		return true;
	}
	return GameManager::getInstance().ballDownCollisionFlag;
}