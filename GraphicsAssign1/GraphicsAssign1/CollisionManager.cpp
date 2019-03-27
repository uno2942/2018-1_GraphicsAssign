#include "GameManager.h"
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
void GameManager::CollisionManager::CollisionHandler(vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>* collisionPairvector)
{
	while (collisionPairvector != NULL && !(collisionPairvector->empty())) {
		if (collisionPairvector->back().first.first->shape == Object::Shape::OVAL)
		{
			Object* o1 = collisionPairvector->back().first.first;
			Object* o2 = collisionPairvector->back().first.second;
			if (
				(collisionPairvector->back().second.x == 0 && collisionPairvector->back().second.y > 0 && collisionwithballmap[o2->name]==3) ||
				(collisionPairvector->back().second.x == 0 && collisionPairvector->back().second.y < 0 && collisionwithballmap[o2->name] == 4) ||
				(collisionPairvector->back().second.x > 0 && collisionwithballmap[o2->name] == 1) ||
				(collisionPairvector->back().second.x < 0 && collisionwithballmap[o2->name] == 2)
				)
			{
				collisionPairvector->pop_back();
				continue;
			}
		}
		if (collisionPairvector->back().first.first->shape == Object::Shape::OVAL)
		{
			if (!(Vector2::abs(collisionPairvector->back().first.first->velocity + collisionPairvector->back().second) > GameManager::BALL_VELOCITY - 1 &&
				Vector2::abs(collisionPairvector->back().first.first->velocity + collisionPairvector->back().second) < GameManager::BALL_VELOCITY + 1))
			{
				collisionPairvector->pop_back();
				continue;
			}
		}
		collisionPairvector->back().first.first->velocity += collisionPairvector->back().second;

		collisionPairvector->pop_back();
	}
}


//모든 게임 오브젝트 사이에 Collision을 체크함
vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>* GameManager::CollisionManager::CollisionCheck() {
	collisionwithballmap["player"] = 0;
	collisionwithballmap["enemy"] = 0;
	collisionwithballmap["net"] = 0;
	collisionwithballmap["leftwall"] = 0;
	collisionwithballmap["rightwall"] = 0;
	collisionwithballmap["topwall"] = 0;


	if (GameManager::getInstance().ball->position.y <= YBORDER)
	{
		if (GameManager::getInstance().ball->position.x + (GameManager::getInstance().ball->width / 2) <= GameManager::getInstance().net->position.x)
		{
			GameManager::getInstance().OneGameEnd(false);
			return collisionPairvector;
		}
		else if (GameManager::getInstance().ball->position.x + (GameManager::getInstance().ball->GetSize().x / 2) >= GameManager::getInstance().net->position.x + GameManager::getInstance().net->width)
		{
			GameManager::getInstance().OneGameEnd(true);
			return collisionPairvector;
		}
		else
			throw;
	}
	else
	{
		for (int i = 0; i < collisionList.size(); i++) {
			for (int j = i + 1; j < collisionList.size(); j++)
				CheckCollision4side(&collisionList[i], &collisionList[j], collisionPairvector);
		}
		return collisionPairvector;
	}
}

/**
return t...
**/
vector<pair<pair<Object*, Object*>, Vector2>>*  GameManager::CollisionManager::RestoreBallPosition(vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>* collisionPairvector) {
	ballDeltaTime = 0;
	if (!collisionPairvector->empty()) {
		GLdouble ballCollideWithCorner = -1;
		GLdouble deltaTime = GameManager::getInstance().DeltaTime();
		vector<pair<pair<Object*, Object*>, Vector2>>::reverse_iterator iter = collisionPairvector->rbegin();
		int l = collisionPairvector->size();
		for (int i = 0; i < l; i++)
		{
			Object* o1 = iter[i].first.first;
			Object* o2 = iter[i].first.second;
			if (iter[i].first.first->shape == Object::Shape::OVAL) {
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
							o1->position += (o1->velocity) * ((-(ballDeltaTime - prevBallDeltaTime)));

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
void GameManager::CollisionManager::CheckCollision4side(const CollisionComponent& o1, const CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>* collisionPairvector) {
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
o1이 o2를 오른쪽에서 충돌
**/
bool GameManager::CollisionManager::CheckCollisionAtRightSide(const CollisionComponent& o1, const CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>* collisionPairvector) {

	if (Object::Shape::BOX == o1.collisionObject->shape && Object::Shape::BOX == o2.collisionObject->shape)
	{
		if (o1.GetWorldPos().x < o2.GetWorldPos().x && o1.GetWorldPos().x + o1.GetWidth() >= o2.GetWorldPos().x)
		{
			collisionPairvector->push_back(make_pair(make_pair(o1.gameObjectNode, o1.gameObjectNode), Vector2(-1, 0)));
			collisionPairvector->push_back(make_pair(make_pair(o2.gameObjectNode, o1.gameObjectNode), Vector2(1, 0)));
			return true;
		}
		return false;
	}
	else if (Object::Shape::OVAL == o1.collisionObject->shape && Object::Shape::BOX == o2.collisionObject->shape) {
		if (o1.GetWorldPos().x < o2.GetWorldPos().x &&  o1.GetWorldPos().x + o1.GetWidth() >= o2.GetWorldPos().x &&
			(o1.GetWorldPos().y + (o1.GetHeight() / 2) >= o2.GetWorldPos().y && o1.GetWorldPos().y + (o1.GetHeight() / 2) <= o2.GetWorldPos().y + o2.GetHeight()))
		{
			collisionPairvector->push_back(make_pair(make_pair(o1.gameObjectNode, o1.gameObjectNode), Vector2(-1, 0)));
			collisionPairvector->push_back(make_pair(make_pair(o1.gameObjectNode, o1.gameObjectNode), Vector2(1, 0)));
			return true;
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
					Vector2::normalize(centertocorner)));
				collisionPairvector->push_back(make_pair(make_pair(o2, o1),
					-Vector2::normalize(centertocorner)));
				return true;
			}
		}
	}
	return false;
}

/**
o1이 o2를 왼쪽에서 충돌
**/
bool GameManager::CollisionManager::CheckCollisionAtLeftSide(const CollisionComponent& o1, const CollisionComponent& o2, vector<pair<pair<Object*, Object*>, Vector2>>* collisionPairvector) {
	if (Object::Shape::BOX == o1->shape && Object::Shape::BOX == o2->shape)
	{
		if (o1->position.x + o1->width > o2->position.x + o2->width && o1->position.x <= o2->position.x + o2->width)
		{
			collisionPairvector->push_back(make_pair(make_pair(o1, o2), Vector2(1, 0)));
			collisionPairvector->push_back(make_pair(make_pair(o2, o1), Vector2(-1, 0)));
			return true;
		}
		return false;
	}
	else if (Object::Shape::OVAL == o1->shape && Object::Shape::BOX == o2->shape) {
		if (o1->position.x + o1->width > o2->position.x + o2->width && o1->position.x <= o2->position.x + o2->width &&
			(o1->position.y + (o1->height / 2) >= o2->position.y && o1->position.y + (o1->height / 2) <= o2->position.y + o2->height))
		{
			collisionPairvector->push_back(make_pair(make_pair(o1, o2), Vector2(1, 0)));
			collisionPairvector->push_back(make_pair(make_pair(o2, o1), Vector2(-1, 0)));
			return true;
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
					Vector2::normalize(centertocorner)));
				collisionPairvector->push_back(make_pair(make_pair(o2, o1),
					-Vector2::normalize(centertocorner)));
				return true;
			}
		}
	}
	return false;
}



/**
**/
bool GameManager::CollisionManager::CheckCollisionAtUpSide(const CollisionComponent& o1, const CollisionComponent& o2, vector<pair<pair<Object*, Object*>, Vector2>>* collisionPairvector) {
	if (Object::Shape::OVAL == o1->shape && Object::Shape::BOX == o2->shape)
	{
		if (o1->position.y < o2->position.y && o1->position.y + o1->height >= o2->position.y)
		{
			collisionPairvector->push_back(make_pair(make_pair(o1, o2), Vector2(0, -1)));
			collisionPairvector->push_back(make_pair(make_pair(o2, o1), Vector2(0, 1)));
			return true;
		}
	}
	return false;
}



/**
**/
bool GameManager::CollisionManager::CheckCollisionAtDownSide(const CollisionComponent& o1, const CollisionComponent& o2, vector<pair<pair<Object*, Object*>, Vector2>>* collisionPairvector) {
	if (Object::Shape::OVAL == o1->shape && Object::Shape::BOX == o2->shape)
	{
		if (o1->position.y < o2->position.y + o2->height && o1->position.y + o1->height >= o2->position.y + o2->height &&
			(o1->position.x + (o1->width / 2) >= o2->position.x && o1->position.x + (o1->width / 2) <= o2->position.x + o2->width))
		{
			collisionPairvector->push_back(make_pair(make_pair(o1, o2), Vector2(0, 1)));
			collisionPairvector->push_back(make_pair(make_pair(o2, o1), Vector2(0, -1)));
			return true;
		}
	}
	return false;
}