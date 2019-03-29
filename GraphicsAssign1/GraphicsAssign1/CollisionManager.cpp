#include "GameManager.h"
using namespace std;

void GameManager::CollisionManager::PutCollisionObject(CollisionComponent* collisionComponent) {
	collisionList->push_back(collisionComponent);
}

/*void GameManager::CollisionManager::RemoveCollisionObjectInObjectNode(GameObject gameObject) {
	for (int i = 0; i < collisionList->size(); i++) //error will occur
		if ((*collisionList)[i].gameObjectNode->data.object == gameObject.object)
			collisionList->erase(collisionList->begin() + i);
}*/

vector<CollisionComponent*>* GameManager::CollisionManager::collisionList = new vector<CollisionComponent*>();
GameManager::CollisionManager::CollisionManager() {
	collisionPairvector = new vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>();
	collisionwithballmap["player"] = 0;
	collisionwithballmap["enemy"] = 0;
	collisionwithballmap["net"] = 0;
	collisionwithballmap["leftwall"] = 0;
	collisionwithballmap["rightwall"] = 0;
	collisionwithballmap["topwall"] = 0;
}
GameManager::CollisionManager::~CollisionManager() {
	if (collisionList != NULL)
	{
		delete collisionList;
		collisionList = NULL;
	}
	if (collisionPairvector != NULL)
	{
		delete collisionPairvector;
		collisionPairvector = NULL;
	}
}
//공의 경우 vector normalization 체크
void GameManager::CollisionManager::CollisionHandler(vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>* collisionPairvector)
{
	while (collisionPairvector != NULL && !(collisionPairvector->empty())) {
		Object* o1 = collisionPairvector->back().first.first->data->object;
		Object* o2 = collisionPairvector->back().first.second->data->object;
		if (o1->shape == Object::Shape::OVAL)
		{
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
		else if (collisionPairvector->back().first.first->data->object->shape == Object::Shape::OVAL)
		{
			if (!(Vector2::abs(o1->velocity + collisionPairvector->back().second) > GameManager::BALL_VELOCITY - 1 &&
				Vector2::abs(o1->velocity + collisionPairvector->back().second) < GameManager::BALL_VELOCITY + 1))
			{
				collisionPairvector->pop_back();
				continue;
			}
		}
		if (o1->name == "leftwall")
		{
			collisionPairvector->pop_back();
			continue;
		}
		if (o1->shape == Object::Shape::OVAL && o2->name == "playerBox")
			GameManager::getInstance().SetShakeTime();
		o1->velocity += collisionPairvector->back().second;

		collisionPairvector->pop_back();
	}
}


/**
return t...
**/
vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>*  GameManager::CollisionManager::RestoreBallPosition(vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>* collisionPairvector) {
	ballDeltaTime = 0;
	if (!collisionPairvector->empty()) {
		GLdouble ballCollideWithCorner = -1;
		GLdouble deltaTime = GameManager::getInstance().DeltaTime();
		vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>::reverse_iterator iter = collisionPairvector->rbegin();
		int l = collisionPairvector->size();
		for (int i = 0; i < l; i++)
		{
			Object* o1 = iter[i].first.first->data->object; //root만 들어온다고 가정
			Object* o2 = iter[i].first.second->data->object; //root만 들어온다고 가정
			if (o1->shape == Object::Shape::OVAL) {
				//right part
				if (collisionwithballmap[o2->name] == 1) {
					{
						if ( o1->position.x < o2->position.x && o1->position.x + o1->width >= o2->position.x &&
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












//collisionList에 있는 CollisionComponent 사이에 Collision을 체크함
vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>* GameManager::CollisionManager::CollisionCheck() {
	collisionwithballmap["player"] = 0;
	collisionwithballmap["enemy"] = 0;
	collisionwithballmap["net"] = 0;
	collisionwithballmap["leftwall"] = 0;
	collisionwithballmap["rightwall"] = 0;
	collisionwithballmap["topwall"] = 0;


	if (GameManager::getInstance().ball->GetCurrentPosition().y <= YBORDER)
	{
		if (GameManager::getInstance().ball->GetCurrentPosition().x + (GameManager::getInstance().ball->width / 2) <= GameManager::getInstance().net->GetCurrentPosition().x)
		{
			GameManager::getInstance().OneGameEnd(false);
			return collisionPairvector;
		}
		else if (GameManager::getInstance().ball->GetCurrentPosition().x + (GameManager::getInstance().ball->GetSize().x / 2) >= GameManager::getInstance().net->GetCurrentPosition().x + GameManager::getInstance().net->width)
		{
			GameManager::getInstance().OneGameEnd(true);
			return collisionPairvector;
		}
		else
			throw;
	}
	else
	{
		for (int i = 0; i < collisionList->size(); i++) {
			for (int j = i + 1; j < collisionList->size(); j++)
				CheckCollision4side(*(*collisionList)[i], *(*collisionList)[j], collisionPairvector);
		}
		return collisionPairvector;
	}
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

	if (Object::Shape::BOX == o1.GetShape() && Object::Shape::BOX == o2.GetShape())
	{
		if (o1.GetWorldPos().x < o2.GetWorldPos().x && o1.GetWorldPos().x + o1.GetWidth() >= o2.GetWorldPos().x)
		{
			collisionPairvector->push_back(make_pair(make_pair(o1.gameObjectNode, o2.gameObjectNode), Vector2(-1, 0)));
			return true;
		}
		return false;
	}
	else if (Object::Shape::OVAL == o1.GetShape() && Object::Shape::BOX == o2.GetShape()) {
		if (o1.GetWorldPos().x < o2.GetWorldPos().x &&  o1.GetWorldPos().x + o1.GetWidth() >= o2.GetWorldPos().x &&
			(o1.GetWorldPos().y + (o1.GetHeight() / 2) >= o2.GetWorldPos().y && o1.GetWorldPos().y + (o1.GetHeight() / 2) <= o2.GetWorldPos().y + o2.GetHeight()))
		{
			collisionPairvector->push_back(make_pair(make_pair(o1.gameObjectNode, o2.gameObjectNode), Vector2(-2 * (o1.GetVelocity().x), 0)));
			collisionwithballmap[o2.gameObjectNode->data->object->name] = 1;
			return true;
		}
		else {
			Vector2 center = Vector2(o1.GetWorldPos().x + o1.GetWidth() / 2, o1.GetWorldPos().y + o1.GetHeight() / 2);
			Vector2 leftcorner = Vector2(o2.GetWorldPos().x, o2.GetWorldPos().y + o2.GetHeight());
			GLdouble r = o1.GetWidth() / 2;
			Vector2 centertocorner = Vector2(center - leftcorner);
			GLdouble distance = Vector2::abs(centertocorner);
			if (r >= distance)
			{
				collisionPairvector->push_back(make_pair(make_pair(o1.gameObjectNode, o2.gameObjectNode),
					Vector2::normalize(centertocorner)*
					(2 * BALL_VELOCITY*abs(o1.GetVelocity().x*centertocorner.x + o1.GetVelocity().y*centertocorner.y) /
					(Vector2::abs(o1.GetVelocity())*Vector2::abs(centertocorner)))));
				collisionwithballmap[o2.gameObjectNode->data->object->name] = 1;
				return true;
			}
		}
	}
	return false;
}

/**
o1이 o2를 왼쪽에서 충돌
**/
bool GameManager::CollisionManager::CheckCollisionAtLeftSide(const CollisionComponent& o1, const CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>* collisionPairvector) {
	if (Object::Shape::BOX == o1.GetShape() && Object::Shape::BOX == o2.GetShape())
	{
		if (o1.GetWorldPos().x + o1.GetWidth() > o2.GetWorldPos().x + o2.GetWidth() && o1.GetWorldPos().x <= o2.GetWorldPos().x + o2.GetWidth())
		{
			collisionPairvector->push_back(make_pair(make_pair(o1.gameObjectNode, o2.gameObjectNode), Vector2(1, 0)));
			return true;
		}
		return false;
	}
	else if (Object::Shape::OVAL == o1.GetShape() && Object::Shape::BOX == o2.GetShape()) {
		if (o1.GetWorldPos().x + o1.GetWidth() > o2.GetWorldPos().x + o2.GetWidth() && o1.GetWorldPos().x <= o2.GetWorldPos().x + o2.GetWidth() &&
			(o1.GetWorldPos().y + (o1.GetHeight() / 2) >= o2.GetWorldPos().y && o1.GetWorldPos().y + (o1.GetHeight() / 2) <= o2.GetWorldPos().y + o2.GetHeight()))
		{
			collisionPairvector->push_back(make_pair(make_pair(o1.gameObjectNode, o2.gameObjectNode), Vector2(-2 * (o1.GetVelocity().x), 0)));
			collisionwithballmap[o2.gameObjectNode->data->object->name] = 2;
			return true;
		}
		else {
			Vector2 center = Vector2(o1.GetWorldPos().x + o1.GetWidth() / 2, o1.GetWorldPos().y + o1.GetHeight() / 2);
			Vector2 rightcorner = Vector2(o2.GetWorldPos().x + o2.GetWidth(), o2.GetWorldPos().y + o2.GetHeight());
			GLdouble r = o1.GetWidth() / 2;
			Vector2 centertocorner = Vector2(center - rightcorner);
			GLdouble distance = Vector2::abs(centertocorner);
			if (r >= distance)
			{
				collisionPairvector->push_back(make_pair(make_pair(o1.gameObjectNode, o2.gameObjectNode),
					Vector2::normalize(centertocorner)*
					(2 * BALL_VELOCITY*abs(o1.GetVelocity().x*centertocorner.x + o1.GetVelocity().y*centertocorner.y) /
					(Vector2::abs(o1.GetVelocity())*distance))));
				collisionwithballmap[o2.gameObjectNode->data->object->name] = 2;
				return true;
			}
		}
	}
	return false;
}



/**
**/
bool GameManager::CollisionManager::CheckCollisionAtUpSide(const CollisionComponent& o1, const CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>* collisionPairvector) {
	if (Object::Shape::OVAL == o1.GetShape() && Object::Shape::BOX == o2.GetShape())
	{
		if (o1.GetWorldPos().y < o2.GetWorldPos().y && o1.GetWorldPos().y + o1.GetHeight() >= o2.GetWorldPos().y)
		{
			collisionPairvector->push_back(make_pair(make_pair(o1.gameObjectNode, o2.gameObjectNode), Vector2(0, -2 * (o1.GetVelocity().y))));
			collisionwithballmap[o2.gameObjectNode->data->object->name] = 3;
			return true;
		}
	}
	else {
		return true;
	}
	return false;
}



/**
**/
bool GameManager::CollisionManager::CheckCollisionAtDownSide(const CollisionComponent& o1, const CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector2>>* collisionPairvector) {
	if (Object::Shape::OVAL == o1.GetShape() && Object::Shape::BOX == o2.GetShape())
	{
		if (o1.GetWorldPos().y < o2.GetWorldPos().y + o2.GetHeight() && o1.GetWorldPos().y + o1.GetHeight() >= o2.GetWorldPos().y + o2.GetHeight() &&
			(o1.GetWorldPos().x + (o1.GetWidth() / 2) >= o2.GetWorldPos().x && o1.GetWorldPos().x + (o1.GetWidth() / 2) <= o2.GetWorldPos().x + o2.GetWidth()))
		{
			collisionPairvector->push_back(make_pair(make_pair(o1.gameObjectNode, o2.gameObjectNode), Vector2(0, -2 * (o1.GetVelocity().y))));			collisionwithballmap[o2.gameObjectNode->data->object->name] = 4;
			return true;
		}
	}
	else {
		return true;
	}
	return false;
}