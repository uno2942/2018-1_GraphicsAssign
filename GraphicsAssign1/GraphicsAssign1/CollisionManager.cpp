#include "GameManager.h"
#include<ctime>
#include<cmath>
using namespace std;
GameManager::CollisionManager::CollisionManager() {
	collisionPairvector = new vector<pair<pair<Object*, Object*>, Vector2>>();
	collisionwithballmap["player"] = 0;
	collisionwithballmap["enemy"] = 0;
	collisionwithballmap["net"] = 0;
	collisionwithballmap["leftwall"] = 0;
	collisionwithballmap["rightwall"] = 0;
	collisionwithballmap["topwall"] = 0;
}

//공의 경우 vector normalization 체크
void GameManager::CollisionManager::CollisionHandler(vector<pair<pair<Object*, Object*>, Vector2>>* collisionPairvector)
{
	bool doubleCollision = (!GameManager::getInstance().ballUpCollisionFlag || GameManager::getInstance().ballDownCollisionFlag || GameManager::getInstance().ballRightCollisionFlag || GameManager::getInstance().ballLeftCollisionFlag) &&

		(GameManager::getInstance().ballUpCollisionFlag || !GameManager::getInstance().ballDownCollisionFlag || GameManager::getInstance().ballRightCollisionFlag || GameManager::getInstance().ballLeftCollisionFlag) &&

		(GameManager::getInstance().ballUpCollisionFlag || GameManager::getInstance().ballDownCollisionFlag || !GameManager::getInstance().ballRightCollisionFlag && GameManager::getInstance().ballLeftCollisionFlag) &&

		(GameManager::getInstance().ballUpCollisionFlag || GameManager::getInstance().ballDownCollisionFlag || GameManager::getInstance().ballRightCollisionFlag || !GameManager::getInstance().ballLeftCollisionFlag);


	while (collisionPairvector != NULL && !(collisionPairvector->empty())) {
		if (!doubleCollision && collisionPairvector->back().first.first->shape == Object::Shape::CIRCLE)
		{
			Object* o1 = collisionPairvector->back().first.first;
			Object* o2 = collisionPairvector->back().first.second;
			if (
				(collisionPairvector->back().second.x == 0 && collisionPairvector->back().second.y > 0 && collisionwithballmap[o2->name]==3) ||
				(collisionPairvector->back().second.x == 0 && collisionPairvector->back().second.y < 0 && collisionwithballmap[o2->name] == 4) ||
				(collisionPairvector->back().second.x > 0 && collisionwithballmap[o2->name] == 1) ||
				(collisionPairvector->back().second.x < 0 && collisionwithballmap[o2->name] == 4)
				)
			{
				collisionPairvector->pop_back();
				continue;
			}
		}
		if (collisionPairvector->back().first.first->shape == Object::Shape::CIRCLE)
		{
			if (!(Vector2::abs(collisionPairvector->back().first.first->velocity + collisionPairvector->back().second) > BALL_VELOCITY - 1 &&
				Vector2::abs(collisionPairvector->back().first.first->velocity + collisionPairvector->back().second) < BALL_VELOCITY + 1))
			{
				collisionPairvector->pop_back();
				continue;
			}
		}
		collisionPairvector->back().first.first->velocity += collisionPairvector->back().second;

		if (collisionPairvector->back().first.first->shape == Object::Shape::CIRCLE)
			cout << "Ball Velocity: " << Vector2::abs(collisionPairvector->back().first.first->velocity) << endl;
		collisionPairvector->pop_back();
	}
}


//모든 게임 오브젝트 사이에 Collision을 체크함
vector<pair<pair<Object*, Object*>, Vector2>>* GameManager::CollisionManager::CollisionCheck() {
	collisionwithballmap["player"] = 0;
	collisionwithballmap["enemy"] = 0;
	collisionwithballmap["net"] = 0;
	collisionwithballmap["leftwall"] = 0;
	collisionwithballmap["rightwall"] = 0;
	collisionwithballmap["topwall"] = 0;
	if (GameManager::getInstance().ball.position.y <= GameManager::getInstance().screen.position.y)
	{
		if (GameManager::getInstance().ball.position.x + (GameManager::getInstance().ball.width / 2) <= GameManager::getInstance().net.position.x)
		{
			GameManager::getInstance().OneGameEnd(false);
			return collisionPairvector;
		}
		else if (GameManager::getInstance().ball.position.x + (GameManager::getInstance().ball.GetSize().x / 2) >= GameManager::getInstance().net.position.x + GameManager::getInstance().net.width)
		{
			GameManager::getInstance().OneGameEnd(true);
			return collisionPairvector;
		}
		else
			throw;
	}
	else
	{
		CheckCollisionAtRightSide(&(GameManager::getInstance().playerBox), &GameManager::getInstance().net, collisionPairvector);
		CheckCollisionAtLeftSide(&GameManager::getInstance().playerBox, &GameManager::getInstance().leftwall, collisionPairvector);

		CheckCollisionAtLeftSide(&GameManager::getInstance().enemyBox, &GameManager::getInstance().net, collisionPairvector);
		CheckCollisionAtRightSide(&GameManager::getInstance().enemyBox, &GameManager::getInstance().rightwall, collisionPairvector);

		CheckCollision4side(&GameManager::getInstance().ball, &GameManager::getInstance().playerBox, collisionPairvector);
		CheckCollision4side(&GameManager::getInstance().ball, &GameManager::getInstance().enemyBox, collisionPairvector);
		CheckCollision4side(&GameManager::getInstance().ball, &GameManager::getInstance().net, collisionPairvector);
		CheckCollisionAtLeftSide(&GameManager::getInstance().ball, &GameManager::getInstance().leftwall, collisionPairvector);
		CheckCollisionAtRightSide(&GameManager::getInstance().ball, &GameManager::getInstance().rightwall, collisionPairvector);
		CheckCollisionAtUpSide(&GameManager::getInstance().ball, &GameManager::getInstance().topwall, collisionPairvector);
		return collisionPairvector;
	}
}

/**
return t...
**/
vector<pair<pair<Object*, Object*>, Vector2>>*  GameManager::CollisionManager::RestoreBallPosition(vector<pair<pair<Object*, Object*>, Vector2>>* collisionPairvector) {
	if (!collisionPairvector->empty()) {
		ballDeltaTime = 0;
		GLdouble ballCollideWithCorner = -1;
		GLdouble deltaTime = GameManager::getInstance().DeltaTime();
		vector<pair<pair<Object*, Object*>, Vector2>>::reverse_iterator iter = collisionPairvector->rbegin();
		int l = collisionPairvector->size();
		for (int i = 0; i < l; i++)
		{
			Object* o1 = iter[i].first.first;
			Object* o2 = iter[i].first.second;
			if (iter[i].first.first->shape == Object::Shape::CIRCLE) {
				//right part
				if (collisionwithballmap[o2->name] == 1) {
					{
						if (o1->position.x < o2->position.x && o1->position.x + o1->width >= o2->position.x &&
							(o1->position.y + (o1->height / 2) >= o2->position.y && o1->position.y + (o1->height / 2) <= o2->position.y + o2->height))
						{
							GLdouble t = (o1->position.x + o1->width - o2->position.x) / abs(o1->velocity.x);
							if (t < 0)
								t = 0;
							GLdouble prevBallDeltaTime = ballDeltaTime;
							ballDeltaTime += t;
							if (ballDeltaTime > deltaTime) ballDeltaTime = deltaTime;

							o1->position += (o1->velocity) * ((-(ballDeltaTime - prevBallDeltaTime)));
							o1->position.x = o2->position.x - o1->width;

						}
						else {
							Vector2 center = Vector2(o1->position.x + o1->width / 2, o1->position.y + o1->height / 2);
							Vector2 leftcorner = Vector2(o2->position.x, o2->position.y + o2->height);
							GLdouble r = o1->width / 2;
							Vector2 centertocorner = Vector2(center - leftcorner);
							GLdouble distance = Vector2::abs(centertocorner);
							if (r >= distance)
							{
								GLdouble innerprod = (o1->velocity.x*centertocorner.x + o1->velocity.y*centertocorner.y);
								GLdouble t = (innerprod + sqrt(innerprod*innerprod - (Vector2::abs(o1->velocity)*Vector2::abs(o1->velocity)*(distance*distance - r * r)))) / (Vector2::abs(o1->velocity)*Vector2::abs(o1->velocity));
								if (t < 0)
									t = 0;
								GLdouble prevBallDeltaTime = ballDeltaTime;
								ballDeltaTime += t;
								if (ballDeltaTime > deltaTime) {
									ballDeltaTime = deltaTime;
								}
								//						GLdouble ballCollideWithCorner = ballDeltaTime;
								o1->position += (o1->velocity) * ((-(ballDeltaTime - prevBallDeltaTime)));
							}
						}
					}
				}

				//left part
				else if (collisionwithballmap[o2->name] == 2)
				{
					if (o1->position.x + o1->width > o2->position.x + o2->width && o1->position.x <= o2->position.x + o2->width &&
						(o1->position.y + (o1->height / 2) >= o2->position.y && o1->position.y + (o1->height / 2) <= o2->position.y + o2->height))
					{
						GLdouble t = (o2->position.x + o2->width - o1->position.x) / abs(o1->velocity.x);
						if (t < 0)
							t = 0;
						GLdouble prevBallDeltaTime = ballDeltaTime;
						ballDeltaTime += t;
						if (ballDeltaTime > deltaTime) ballDeltaTime = deltaTime;

						o1->position += (o1->velocity) * ((-(ballDeltaTime - prevBallDeltaTime)));
						o1->position.x = o2->position.x + o2->width;

					}
					else {
						Vector2 center = Vector2(o1->position.x + o1->width / 2, o1->position.y + o1->height / 2);
						Vector2 rightcorner = Vector2(o2->position.x + o2->width, o2->position.y + o2->height);
						GLdouble r = o1->width / 2;
						Vector2 centertocorner = Vector2(center - rightcorner);
						GLdouble distance = Vector2::abs(centertocorner);
						if (r >= distance)
						{
							GLdouble innerprod = (o1->velocity.x*centertocorner.x + o1->velocity.y*centertocorner.y);
							GLdouble t = (innerprod + sqrt(innerprod*innerprod - (Vector2::abs(o1->velocity)*Vector2::abs(o1->velocity)*(distance*distance - r * r)))) / (Vector2::abs(o1->velocity)*Vector2::abs(o1->velocity));
							if (t < 0)
								t = 0;
							GLdouble prevBallDeltaTime = ballDeltaTime;
							ballDeltaTime += t;
							if (ballDeltaTime > deltaTime) {
								ballDeltaTime = deltaTime;
							}
							//						GLdouble ballCollideWithCorner = ballDeltaTime;
							cout << "dist Prev: " << distance;
							o1->position += (o1->velocity) * ((-(ballDeltaTime - prevBallDeltaTime)));
							Vector2 center2 = Vector2(o1->position.x + o1->width / 2, o1->position.y + o1->height / 2);
							Vector2 centertocorner2 = Vector2(center2 - rightcorner);
							GLdouble distance2 = Vector2::abs(centertocorner2);
							cout << "dist Now: " << distance2;
						}
					}

				}
				//up part
				else if (collisionwithballmap[o2->name] == 3)
				{
					if (o1->position.y < o2->position.y && o1->position.y + o1->height >= o2->position.y)
					{
						GLdouble t = (o1->position.y + o1->height - o2->position.y) / abs(o1->velocity.y);
						if (t < 0)
							t = 0;
						GLdouble prevBallDeltaTime = ballDeltaTime;
						ballDeltaTime += t;
						if (ballDeltaTime > deltaTime) ballDeltaTime = deltaTime;

						o1->position += (o1->velocity) * ((-(ballDeltaTime - prevBallDeltaTime)));
						o1->position.y = o2->position.y - o1->height;

					}
				}
				//down part
				else if (collisionwithballmap[o2->name] == 4) {
					if (o1->position.y < o2->position.y + o2->height && o1->position.y + o1->height >= o2->position.y + o2->height &&
						(o1->position.x + (o1->width / 2) >= o2->position.x && o1->position.x + (o1->width / 2) <= o2->position.x + o2->width))
					{
						GLdouble t = (o2->position.y + o2->height - o1->position.y) / abs(o1->velocity.y);
						if (t < 0)
							t = 0;
						GLdouble prevBallDeltaTime = ballDeltaTime;
						ballDeltaTime += t;
						if (ballDeltaTime > deltaTime) ballDeltaTime = deltaTime;
						o1->position += (o1->velocity) * ((-(ballDeltaTime - prevBallDeltaTime)));
						o1->position.y = o2->position.y + o2->height;

					}
				}
			}
		}
	}
	return collisionPairvector;
}


//두 오브젝트 사이에 Collision을 체크함
void GameManager::CollisionManager::CheckCollision4side(Object* o1, Object* o2, vector<pair<pair<Object*, Object*>, Vector2>>* collisionPairvector) {
	if (!CheckCollisionAtRightSide(o1, o2, collisionPairvector))
	{
		if (!CheckCollisionAtLeftSide(o1, o2, collisionPairvector))
		{
			if (!CheckCollisionAtDownSide(o1, o2, collisionPairvector))
			{
				if (!CheckCollisionAtUpSide(o1, o2, collisionPairvector))
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
bool GameManager::CollisionManager::CheckCollisionAtRightSide(Object* o1, Object* o2, vector<pair<pair<Object*, Object*>, Vector2>>* collisionPairvector) {

	if (Object::Shape::BOX == o1->shape && Object::Shape::BOX == o2->shape)
	{
		if (o1->position.x < o2->position.x && o1->position.x + o1->width >= o2->position.x)
		{
			collisionPairvector->push_back(make_pair(make_pair(o1, o2), Vector2(-1, 0)));
			return true;
		}
		return false;
	}
	else if (Object::Shape::CIRCLE == o1->shape && Object::Shape::BOX == o2->shape) {
		if (o1->position.x < o2->position.x && o1->position.x + o1->width >= o2->position.x &&
			(o1->position.y + (o1->height / 2) >= o2->position.y && o1->position.y + (o1->height / 2) <= o2->position.y + o2->height))
		{
			collisionPairvector->push_back(make_pair(make_pair(o1, o2), Vector2(-2 * (o1->velocity.x), 0)));
			collisionwithballmap[o2->name] = 1;
		}
		else {
			Vector2 center = Vector2(o1->position.x + o1->width / 2, o1->position.y + o1->height / 2);
			Vector2 leftcorner = Vector2(o2->position.x, o2->position.y + o2->height);
			GLdouble r = o1->width / 2;
			Vector2 centertocorner = Vector2(center - leftcorner);
			GLdouble distance = Vector2::abs(centertocorner);
			if (r >= distance)
			{
				collisionPairvector->push_back(make_pair(make_pair(o1, o2),
					Vector2::normalize(centertocorner)*
					(2 * BALL_VELOCITY*abs(o1->velocity.x*centertocorner.x + o1->velocity.y*centertocorner.y) /
					(Vector2::abs(o1->velocity)*Vector2::abs(centertocorner)))));
				cout << "Right" << Vector2::normalize(centertocorner)*
					(2 * BALL_VELOCITY*abs(o1->velocity.x*centertocorner.x + o1->velocity.y*centertocorner.y) /
					(Vector2::abs(o1->velocity)*distance)) << "cos len: " << abs(o1->velocity.x*centertocorner.x + o1->velocity.y*centertocorner.y) /
						(Vector2::abs(o1->velocity)*distance) << endl;
				collisionwithballmap[o2->name] = 1;
			}
			else {

			}
		}
	}
	else {

	}
	return (collisionwithballmap[o2->name] == 1);
}

/**
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
o1이 o2를 왼쪽에서 충돌
**/
bool GameManager::CollisionManager::CheckCollisionAtLeftSide(Object* o1, Object* o2, vector<pair<pair<Object*, Object*>, Vector2>>* collisionPairvector) {
	if (Object::Shape::BOX == o1->shape && Object::Shape::BOX == o2->shape)
	{
		if (o1->position.x + o1->width > o2->position.x + o2->width && o1->position.x <= o2->position.x + o2->width)
		{
			collisionPairvector->push_back(make_pair(make_pair(o1, o2), Vector2(1, 0)));
			return true;
		}
		return false;
	}
	else if (Object::Shape::CIRCLE == o1->shape && Object::Shape::BOX == o2->shape) {
		if (o1->position.x + o1->width > o2->position.x + o2->width && o1->position.x <= o2->position.x + o2->width &&
			(o1->position.y + (o1->height / 2) >= o2->position.y && o1->position.y + (o1->height / 2) <= o2->position.y + o2->height))
		{
			collisionPairvector->push_back(make_pair(make_pair(o1, o2), Vector2(-2 * (o1->velocity.x), 0)));
			collisionwithballmap[o2->name] = 2;
		}
		else {
			Vector2 center = Vector2(o1->position.x + o1->width / 2, o1->position.y + o1->height / 2);
			Vector2 rightcorner = Vector2(o2->position.x + o2->width, o2->position.y + o2->height);
			GLdouble r = o1->width / 2;
			Vector2 centertocorner = Vector2(center - rightcorner);
			GLdouble distance = Vector2::abs(centertocorner);
			if (r >= distance)
			{
				collisionPairvector->push_back(make_pair(make_pair(o1, o2),
					Vector2::normalize(centertocorner)*
					(2 * BALL_VELOCITY*abs(o1->velocity.x*centertocorner.x + o1->velocity.y*centertocorner.y) /
					(Vector2::abs(o1->velocity)*distance))));
				cout << "Left" << Vector2::normalize(centertocorner)*
					(2 * BALL_VELOCITY*abs(o1->velocity.x*centertocorner.x + o1->velocity.y*centertocorner.y) /
					(Vector2::abs(o1->velocity)*distance)) << "cos len: " << abs(o1->velocity.x*centertocorner.x + o1->velocity.y*centertocorner.y) /
						(Vector2::abs(o1->velocity)*distance) << endl;
				collisionwithballmap[o2->name] = 2;
			}
			else
			{
			}
		}
	}
	else
	{

	}
	return (collisionwithballmap[o2->name] == 2);
}



/**
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
**/
bool GameManager::CollisionManager::CheckCollisionAtUpSide(Object* o1, Object* o2, vector<pair<pair<Object*, Object*>, Vector2>>* collisionPairvector) {
	if (Object::Shape::CIRCLE == o1->shape && Object::Shape::BOX == o2->shape)
	{
		if (o1->position.y < o2->position.y && o1->position.y + o1->height >= o2->position.y)
		{
			collisionPairvector->push_back(make_pair(make_pair(o1, o2), Vector2(0, -2 * (o1->velocity.y))));
			collisionwithballmap[o2->name] = 3;
		}
		else
		{
		}
	}
	else
	{
		return true;
	}
	return (collisionwithballmap[o2->name] == 3);
}



/**
게임잼처럼 막 짠 코드라 후에 고칠 필요가 있습니다.
**/
bool GameManager::CollisionManager::CheckCollisionAtDownSide(Object* o1, Object* o2, vector<pair<pair<Object*, Object*>, Vector2>>* collisionPairvector) {
	if (Object::Shape::CIRCLE == o1->shape && Object::Shape::BOX == o2->shape)
	{
		if (o1->position.y < o2->position.y + o2->height && o1->position.y + o1->height >= o2->position.y + o2->height &&
			(o1->position.x + (o1->width / 2) >= o2->position.x && o1->position.x + (o1->width / 2) <= o2->position.x + o2->width))
		{
			collisionPairvector->push_back(make_pair(make_pair(o1, o2), Vector2(0, -2 * (o1->velocity.y))));
			collisionwithballmap[o2->name] = 4;
		}
		else {}
	}
	else {
		return true;
	}
	return (collisionwithballmap[o2->name] == 4);
}