#include"Units.h"

static GameObjectTree* GetBall() {
	static GameObjectTree* ballTree = NULL;
	if (ballTree == NULL)
	{
		ballTree = new GameObjectTree("ball");
		Object* ball = new Oval("ball", 100, 100, MY_RED);
		GameObject* ballObject = new GameObject(ball);
		GameObjectNode* ballNode = new GameObjectNode(ballObject, "ball");
		ballObject->AddCollisionComponentAsItself(ballNode);
		ballTree->insert_back(ballNode);
	}
	return ballTree;
}