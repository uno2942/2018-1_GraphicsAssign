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
	collisionPairvector = new vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>();
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
void GameManager::CollisionManager::CollisionHandler(vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* collisionPairvector)
{/*
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
			if (!(Vector3::abs(o1->velocity + collisionPairvector->back().second) > GameManager::BALL_VELOCITY - 1 &&
				Vector3::abs(o1->velocity + collisionPairvector->back().second) < GameManager::BALL_VELOCITY + 1))
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
		o1->velocity += collisionPairvector->back().second;

		collisionPairvector->pop_back();
	}
	*/
}


/**
return t...
**/
vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>*  GameManager::CollisionManager::RestoreBallPosition(vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* collisionPairvector) {
	/*
	ballDeltaTime = 0;
	if (!collisionPairvector->empty()) {
		GLdouble ballCollideWithCorner = -1;
		GLdouble deltaTime = GameManager::getInstance().DeltaTime();
		vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>::reverse_iterator iter = collisionPairvector->rbegin();
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
							Vector3 center = Vector3(o1->position.x + o1->width / 2, o1->position.y + o1->height / 2);
							Vector3 leftcorner = Vector3(o2->position.x, o2->position.y + o2->height);
							GLdouble r = o1->width / 2;
							Vector3 centertocorner = Vector3(center - leftcorner);
							GLdouble distance = Vector3::abs(centertocorner);
							if (r >= distance)
							{
								GLdouble innerprod = (o1->velocity.x*centertocorner.x + o1->velocity.y*centertocorner.y);
								GLdouble t = (innerprod + sqrt(innerprod*innerprod - (Vector3::abs(o1->velocity)*Vector3::abs(o1->velocity)*(distance*distance - r * r)))) / (Vector3::abs(o1->velocity)*Vector3::abs(o1->velocity));
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
						Vector3 center = Vector3(o1->position.x + o1->width / 2, o1->position.y + o1->height / 2);
						Vector3 rightcorner = Vector3(o2->position.x + o2->width, o2->position.y + o2->height);
						GLdouble r = o1->width / 2;
						Vector3 centertocorner = Vector3(center - rightcorner);
						GLdouble distance = Vector3::abs(centertocorner);
						if (r >= distance)
						{
							GLdouble innerprod = (o1->velocity.x*centertocorner.x + o1->velocity.y*centertocorner.y);
							GLdouble t = (innerprod + sqrt(innerprod*innerprod - (Vector3::abs(o1->velocity)*Vector3::abs(o1->velocity)*(distance*distance - r * r)))) / (Vector3::abs(o1->velocity)*Vector3::abs(o1->velocity));
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
	*/
	return collisionPairvector;
}












//collisionList에 있는 CollisionComponent 사이에 Collision을 체크함
vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* GameManager::CollisionManager::CollisionCheck() {
	collisionwithballmap["player"] = 0;
	collisionwithballmap["enemy"] = 0;
	collisionwithballmap["net"] = 0;
	collisionwithballmap["leftwall"] = 0;
	collisionwithballmap["rightwall"] = 0;
	collisionwithballmap["topwall"] = 0;

	{
		if (GameManager::getInstance().ball->GetCurrentPosition().z - GameManager::getInstance().ball->zlen / 2 <= ZBORDER_FOR_PLAYER)
		{
			GameManager::getInstance().OneGameEnd(true);
			return collisionPairvector;
		}
		else if (GameManager::getInstance().ball->GetCurrentPosition().z + GameManager::getInstance().ball->zlen / 2 >= ZBORDER_FOR_ENEMY)
		{
			GameManager::getInstance().OneGameEnd(false);
			return collisionPairvector;
		}

		if (GameManager::getInstance().player->GetCurrentPosition().x - GameManager::getInstance().player->xlen / 2 < 0
			&& GameManager::getInstance().player->velocity.x<0)
			GameManager::getInstance().player->SetVelocity(0, 0, 0);
		else if (GameManager::getInstance().player->GetCurrentPosition().x + GameManager::getInstance().player->xlen / 2 > WORLD_COORD_MAP_XLEN
			&& GameManager::getInstance().player->velocity.x > 0)
			GameManager::getInstance().player->SetVelocity(0, 0, 0);

		if (GameManager::getInstance().player->GetCurrentPosition().z - GameManager::getInstance().player->zlen / 2 < WORLD_COORD_MAP_ZLEN / 2
			&& GameManager::getInstance().player->velocity.z < 0)
			GameManager::getInstance().player->SetVelocity(0, 0, 0);
		else if (GameManager::getInstance().player->GetCurrentPosition().z + GameManager::getInstance().player->zlen / 2 > WORLD_COORD_MAP_ZLEN
			&& GameManager::getInstance().player->velocity.z > 0)
			GameManager::getInstance().player->SetVelocity(0, 0, 0);

		if (GameManager::getInstance().enemy->GetCurrentPosition().x - GameManager::getInstance().enemy->xlen / 2 < 0
			&& GameManager::getInstance().enemy->velocity.x < 0)
			GameManager::getInstance().enemy->SetVelocity(0, 0, 0);
		else if (GameManager::getInstance().enemy->GetCurrentPosition().x + GameManager::getInstance().enemy->xlen / 2 > WORLD_COORD_MAP_XLEN
			&& GameManager::getInstance().enemy->velocity.x < 0)
			GameManager::getInstance().enemy->SetVelocity(0, 0, 0);

		if (GameManager::getInstance().enemy->GetCurrentPosition().z - GameManager::getInstance().enemy->zlen / 2 > WORLD_COORD_MAP_ZLEN / 2
			&& GameManager::getInstance().enemy->velocity.z > 0)
			GameManager::getInstance().enemy->SetVelocity(0, 0, 0);
		else if (GameManager::getInstance().enemy->GetCurrentPosition().z + GameManager::getInstance().enemy->zlen / 2 <0
			&& GameManager::getInstance().enemy->velocity.z < 0)
			GameManager::getInstance().enemy->SetVelocity(0, 0, 0);
	}

	{
		if (GameManager::getInstance().ball->GetCurrentPosition().x - GameManager::getInstance().ball->xlen / 2 < 0)
		{
			Vector3 ballPos = GameManager::getInstance().ball->GetCurrentPosition();
			Vector3 ballVel = GameManager::getInstance().ball->velocity;
			GameManager::getInstance().ball->SetVelocity(-ballVel.x, ballVel.z, ballVel.y);
			while (GameManager::getInstance().ball->GetCurrentPosition().x - GameManager::getInstance().ball->xlen / 2 < 0)
			{
				GameManager::getInstance().ball->position += GameManager::getInstance().ball->velocity * (0.0001);
			}
			return collisionPairvector;
		}
		else if (GameManager::getInstance().ball->GetCurrentPosition().x + GameManager::getInstance().ball->xlen / 2 > WORLD_COORD_MAP_XLEN)
		{
			Vector3 ballPos = GameManager::getInstance().ball->GetCurrentPosition();
			Vector3 ballVel = GameManager::getInstance().ball->velocity;
			GameManager::getInstance().ball->SetVelocity(-ballVel.x, ballVel.z, ballVel.y);
			while (GameManager::getInstance().ball->GetCurrentPosition().x + GameManager::getInstance().ball->xlen / 2 > WORLD_COORD_MAP_XLEN)
			{
				GameManager::getInstance().ball->position += GameManager::getInstance().ball->velocity * (0.0001);
			}
			return collisionPairvector;
		}
	}

	for (int i = 0; i < collisionList->size(); i++) {
		for (int j = i + 1; j < collisionList->size(); j++)
			CheckCollision4side(*(*collisionList)[i], *(*collisionList)[j], collisionPairvector);
	}
	return collisionPairvector;
}


//두 오브젝트 사이에 Collision을 체크함
void GameManager::CollisionManager::CheckCollision4side(CollisionComponent& o1, CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* collisionPairvector) {

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
bool GameManager::CollisionManager::CheckCollisionAtRightSide(CollisionComponent& o1, CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* collisionPairvector) {
	
	if (Object::Shape::BOX == o1.GetShape() && Object::Shape::OVAL == o2.GetShape())
	{
		return CheckCollisionAtRightSide(o2, o1, collisionPairvector);
	}
	
	Vector2 center = Vector2(o1.GetWorldPos().x, o1.GetWorldPos().z);
	Vector2 leftupcorner = Vector2(o2.GetWorldPos().x - o2.Getxlen() / 2, o2.GetWorldPos().z + o2.Getzlen() / 2);
	Vector2 leftdowncorner = Vector2(o2.GetWorldPos().x - o2.Getxlen() / 2, o2.GetWorldPos().z - o2.Getzlen() / 2);
	GLdouble r = o1.Getxlen() / 2;
	if (Object::Shape::OVAL == o1.GetShape() && Object::Shape::BOX == o2.GetShape()) {
		
		
		if (o1.GetWorldPos().x + o1.Getxlen() / 2 >= o2.GetWorldPos().x - o2.Getxlen()/2 && o1.GetWorldPos().x< o2.GetWorldPos().x - o2.Getxlen() / 2 &&
			(o1.GetWorldPos().z >= o2.GetWorldPos().z - o2.Getzlen()/2 && o1.GetWorldPos().z <= o2.GetWorldPos().z + o2.Getzlen() / 2))
		{
			if (o1.GetVelocity().x > 0)
				o1.gameObjectNode->data->object->velocity += Vector3(-2 * o1.GetVelocity().x, 0, 0);

			while (o1.GetWorldPos().x + o1.Getxlen() / 2 >= o2.GetWorldPos().x - o2.Getxlen() / 2 && o1.GetWorldPos().x < o2.GetWorldPos().x - o2.Getxlen() / 2)
			{
				o1.gameObjectNode->data->object->position += o1.gameObjectNode->data->object->velocity * (0.0001);
			}
			return true;
		}


		else if (o1.GetWorldPos().x + o1.Getxlen() / 2 > o2.GetWorldPos().x - o2.Getxlen() / 2 && o1.GetWorldPos().x < o2.GetWorldPos().x - o2.Getxlen() / 2 &&
			(o1.GetWorldPos().z > o2.GetWorldPos().z + o2.Getzlen() / 2) && Vector2::abs(center-leftupcorner) <= r){
			Vector2 centertocorner = Vector2(center - leftupcorner);
			GLdouble distance = Vector2::abs(centertocorner);
			if (r >= distance)
			{
				Vector2 normalvec =	Vector2::normalize(centertocorner) *
					(BALL_VELOCITY * abs(o1.GetVelocity().x * centertocorner.x + o1.GetVelocity().z * centertocorner.y) /
					(Vector3::abs(o1.GetVelocity()) * Vector2::abs(centertocorner)));

				if (o1.gameObjectNode->data->object->velocity.x * normalvec.x +
					o1.gameObjectNode->data->object->velocity.z * normalvec.y < 0)
				o1.gameObjectNode->data->object->velocity = -o1.gameObjectNode->data->object->velocity + (Vector3(normalvec.x + o1.gameObjectNode->data->object->velocity.x, normalvec.y + o1.gameObjectNode->data->object->velocity.z,0)) * 2;

				while (Vector2::abs(center - leftupcorner) <= r)
				{
					o1.gameObjectNode->data->object->position += o1.gameObjectNode->data->object->velocity * (0.0001);
					center = Vector2(o1.GetWorldPos().x, o1.GetWorldPos().z);
					leftupcorner = Vector2(o2.GetWorldPos().x - o2.Getxlen() / 2, o2.GetWorldPos().z + o2.Getzlen() / 2);
				}
				return true;
			}
		}


		else if (o1.GetWorldPos().x + o1.Getxlen() / 2 > o2.GetWorldPos().x - o2.Getxlen() / 2 && o1.GetWorldPos().x < o2.GetWorldPos().x - o2.Getxlen() / 2 &&
			(o1.GetWorldPos().z < o2.GetWorldPos().z - o2.Getzlen() / 2) && Vector2::abs(center - leftdowncorner) <= r) {
			Vector2 centertocorner = Vector2(center - leftdowncorner);
			GLdouble distance = Vector2::abs(centertocorner);
			if (r >= distance)
			{
				Vector2 normalvec = Vector2::normalize(centertocorner) *
					(BALL_VELOCITY * abs(o1.GetVelocity().x * centertocorner.x + o1.GetVelocity().z * centertocorner.y) /
					(Vector3::abs(o1.GetVelocity()) * Vector2::abs(centertocorner)));
				
				if(o1.gameObjectNode->data->object->velocity.x * normalvec.x +
					o1.gameObjectNode->data->object->velocity.z * normalvec.y < 0)
				o1.gameObjectNode->data->object->velocity = -o1.gameObjectNode->data->object->velocity + (Vector3(normalvec.x + o1.gameObjectNode->data->object->velocity.x, normalvec.y + o1.gameObjectNode->data->object->velocity.z, 0)) * 2;

				while (Vector2::abs(center - leftdowncorner) <= r)
				{
					o1.gameObjectNode->data->object->position += o1.gameObjectNode->data->object->velocity * (0.0001);
					center = Vector2(o1.GetWorldPos().x, o1.GetWorldPos().z);
					leftdowncorner = Vector2(o2.GetWorldPos().x - o2.Getxlen() / 2, o2.GetWorldPos().z - o2.Getzlen() / 2);
				}
				return true;
			}
		}
	}
	return false;
}

/**
o1이 o2를 왼쪽에서 충돌
**/
bool GameManager::CollisionManager::CheckCollisionAtLeftSide(CollisionComponent& o1, CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* collisionPairvector) {

	if (Object::Shape::BOX == o1.GetShape() && Object::Shape::OVAL == o2.GetShape())
	{
		return CheckCollisionAtLeftSide(o2, o1, collisionPairvector);
	}

	Vector2 center = Vector2(o1.GetWorldPos().x, o1.GetWorldPos().z);
	Vector2 rightupcorner = Vector2(o2.GetWorldPos().x + o2.Getxlen() / 2, o2.GetWorldPos().z + o2.Getzlen() / 2);
	Vector2 rightdowncorner = Vector2(o2.GetWorldPos().x + o2.Getxlen() / 2, o2.GetWorldPos().z - o2.Getzlen() / 2);
	GLdouble r = o1.Getxlen() / 2;
	if (Object::Shape::OVAL == o1.GetShape() && Object::Shape::BOX == o2.GetShape()) {


		if (o1.GetWorldPos().x - o1.Getxlen() / 2 <= o2.GetWorldPos().x + o2.Getxlen() / 2 && o1.GetWorldPos().x > o2.GetWorldPos().x + o2.Getxlen() / 2 &&
			(o1.GetWorldPos().z >= o2.GetWorldPos().z - o2.Getzlen() / 2 && o1.GetWorldPos().z <= o2.GetWorldPos().z + o2.Getzlen() / 2))
		{
			if (o1.GetVelocity().x < 0)
				o1.gameObjectNode->data->object->velocity += Vector3(-2 * o1.GetVelocity().x, 0, 0);

			while (o1.GetWorldPos().x - o1.Getxlen() / 2 < o2.GetWorldPos().x + o2.Getxlen() / 2 && o1.GetWorldPos().x >= o2.GetWorldPos().x + o2.Getxlen() / 2)
			{
				o1.gameObjectNode->data->object->position += o1.gameObjectNode->data->object->velocity * (0.0001);
			}
			return true;
		}


		else if (o1.GetWorldPos().x - o1.Getxlen() / 2 < o2.GetWorldPos().x + o2.Getxlen() / 2 && o1.GetWorldPos().x > o2.GetWorldPos().x + o2.Getxlen() / 2 &&
			(o1.GetWorldPos().z > o2.GetWorldPos().z + o2.Getzlen() / 2) && Vector2::abs(center - rightupcorner) <= r) {
			Vector2 centertocorner = Vector2(center - rightupcorner);
			GLdouble distance = Vector2::abs(centertocorner);
			if (r >= distance)
			{
				Vector2 normalvec = Vector2::normalize(centertocorner) *
					(BALL_VELOCITY * abs(o1.GetVelocity().x * centertocorner.x + o1.GetVelocity().z * centertocorner.y) /
					(Vector3::abs(o1.GetVelocity()) * Vector2::abs(centertocorner)));

				if (o1.gameObjectNode->data->object->velocity.x * normalvec.x +
					o1.gameObjectNode->data->object->velocity.z * normalvec.y < 0)
					o1.gameObjectNode->data->object->velocity = -o1.gameObjectNode->data->object->velocity + (Vector3(normalvec.x + o1.gameObjectNode->data->object->velocity.x, normalvec.y + o1.gameObjectNode->data->object->velocity.z, 0)) * 2;

				while (Vector2::abs(center - rightupcorner) <= r)
				{
					o1.gameObjectNode->data->object->position += o1.gameObjectNode->data->object->velocity * (0.0001);
					center = Vector2(o1.GetWorldPos().x, o1.GetWorldPos().z);
					rightupcorner = Vector2(o2.GetWorldPos().x - o2.Getxlen() / 2, o2.GetWorldPos().z + o2.Getzlen() / 2);
				}
				return true;
			}
		}


		else if (o1.GetWorldPos().x - o1.Getxlen() / 2 < o2.GetWorldPos().x + o2.Getxlen() / 2 && o1.GetWorldPos().x > o2.GetWorldPos().x + o2.Getxlen() / 2 &&
			(o1.GetWorldPos().z < o2.GetWorldPos().z - o2.Getzlen() / 2) && Vector2::abs(center - rightdowncorner) <= r) {
			Vector2 centertocorner = Vector2(center - rightdowncorner);
			GLdouble distance = Vector2::abs(centertocorner);
			if (r >= distance)
			{
				Vector2 normalvec = Vector2::normalize(centertocorner) *
					(BALL_VELOCITY * abs(o1.GetVelocity().x * centertocorner.x + o1.GetVelocity().z * centertocorner.y) /
					(Vector3::abs(o1.GetVelocity()) * Vector2::abs(centertocorner)));

				if (o1.gameObjectNode->data->object->velocity.x * normalvec.x +
					o1.gameObjectNode->data->object->velocity.z * normalvec.y < 0)
					o1.gameObjectNode->data->object->velocity = -o1.gameObjectNode->data->object->velocity + (Vector3(normalvec.x + o1.gameObjectNode->data->object->velocity.x, normalvec.y + o1.gameObjectNode->data->object->velocity.z, 0)) * 2;

				while (Vector2::abs(center - rightdowncorner) <= r)
				{
					o1.gameObjectNode->data->object->position += o1.gameObjectNode->data->object->velocity * (0.0001);
					center = Vector2(o1.GetWorldPos().x, o1.GetWorldPos().z);
					rightdowncorner = Vector2(o2.GetWorldPos().x - o2.Getxlen() / 2, o2.GetWorldPos().z - o2.Getzlen() / 2);
				}
				return true;
			}
		}
	}
	return false;
}



/**
**/
bool GameManager::CollisionManager::CheckCollisionAtUpSide(CollisionComponent& o1, CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* collisionPairvector) {
	if (Object::Shape::BOX == o1.GetShape() && Object::Shape::OVAL == o2.GetShape())
	{
		return CheckCollisionAtUpSide(o2, o1, collisionPairvector);
	}

	GLdouble r = o1.gameObjectNode->data->object->zlen / 2;

	if (Object::Shape::OVAL == o1.GetShape() && Object::Shape::BOX == o2.GetShape()) {


		if ( o1.gameObjectNode->data->object->position.x < o2.gameObjectNode->data->object->position.x + o2.gameObjectNode->data->object->xlen/2 &&
			o1.gameObjectNode->data->object->position.x > o2.gameObjectNode->data->object->position.x - o2.gameObjectNode->data->object->xlen / 2&&
			o1.gameObjectNode->data->object->position.z >= o2.gameObjectNode->data->object->position.z + o2.gameObjectNode->data->object->zlen/ 2 &&
			o1.gameObjectNode->data->object->position.z  - r <= o2.gameObjectNode->data->object->position.z + o2.gameObjectNode->data->object->zlen / 2)
		{
			if (o1.GetVelocity().z < 0)
			o1.gameObjectNode->data->object->velocity += Vector3(0, -2 * o1.GetVelocity().z, 0);

			while (o1.gameObjectNode->data->object->position.z >= o2.gameObjectNode->data->object->position.z + o2.gameObjectNode->data->object->zlen / 2 &&
				o1.gameObjectNode->data->object->position.z - r <= o2.gameObjectNode->data->object->position.z + o2.gameObjectNode->data->object->zlen / 2)

				o1.gameObjectNode->data->object->position += o1.gameObjectNode->data->object->velocity * (0.0001);
			}
			return true;
		}
	return false;
}



/**
**/
bool GameManager::CollisionManager::CheckCollisionAtDownSide(CollisionComponent& o1, CollisionComponent& o2, vector<pair<pair<GameObjectNode*, GameObjectNode*>, Vector3>>* collisionPairvector) {
	if (Object::Shape::BOX == o1.GetShape() && Object::Shape::OVAL == o2.GetShape())
	{
		return CheckCollisionAtDownSide(o2, o1, collisionPairvector);
	}

	GLdouble r = o1.gameObjectNode->data->object->zlen / 2;

	if (Object::Shape::OVAL == o1.GetShape() && Object::Shape::BOX == o2.GetShape()) {


		if (o1.gameObjectNode->data->object->position.x < o2.gameObjectNode->data->object->position.x + o2.gameObjectNode->data->object->xlen / 2 &&
			o1.gameObjectNode->data->object->position.x > o2.gameObjectNode->data->object->position.x - o2.gameObjectNode->data->object->xlen / 2 &&
			o1.gameObjectNode->data->object->position.z <= o2.gameObjectNode->data->object->position.z - o2.gameObjectNode->data->object->zlen / 2 &&
			o1.gameObjectNode->data->object->position.z + r >= o2.gameObjectNode->data->object->position.z - o2.gameObjectNode->data->object->zlen / 2)
		{
			if(o1.GetVelocity().z>0)
			o1.gameObjectNode->data->object->velocity += Vector3(0, -2 * o1.GetVelocity().z, 0);

			while (o1.gameObjectNode->data->object->position.z <= o2.gameObjectNode->data->object->position.z - o2.gameObjectNode->data->object->zlen / 2 &&
				o1.gameObjectNode->data->object->position.z + r >= o2.gameObjectNode->data->object->position.z - o2.gameObjectNode->data->object->zlen / 2)

				o1.gameObjectNode->data->object->position += o1.gameObjectNode->data->object->velocity * (0.0001);
			return true;
		}
	}
	return false;
}
