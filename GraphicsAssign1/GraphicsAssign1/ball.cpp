#include"Units.h"

GameObjectTree* GetBall() {
	static GameObjectTree* ballTree = NULL;
	if (ballTree == NULL)
	{
		ballTree = new GameObjectTree("ball");
		Object* ball = new Oval("ball", 100, 100, 100, MY_RED);
		GameObject* ballObject = new GameObject(ball);
		GameObjectNode* ballNode = new GameObjectNode(ballObject, "ball");
		ballObject->AddCollisionComponentAsItself(ballNode);
		ballTree->insert_back(ballNode);
	}
	return ballTree;
}

void InitBallVelocity(int BALL_VELOCITY) {
	double x = 0;
	double y = 0;

	while (0 == x)
		x = rand() % 101 - 50;

	while (-20 < y && y < 20)
		y = rand() % 101 - 50;

	GetBall()->root->data->object->SetVelocity((Vector3::normalize(Vector3(x, y, 0)))*BALL_VELOCITY);
}