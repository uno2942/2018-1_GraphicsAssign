#include"Units.h"

GameObjectTree* GetBall() {
	static GameObjectTree* ballTree = NULL;
	if (ballTree == NULL)
	{
		ballTree = new GameObjectTree("ball");
		Object* ball = new Oval("ball", 200, 200, 200, MY_RED);
		GameObject* ballObject = new GameObject(ball);
		GameObjectNode* ballNode = new GameObjectNode(ballObject, "ball");
		ballObject->AddCollisionComponentAsItself(ballNode);
		ballTree->insert_back(ballNode);
	}
	return ballTree;
}

void InitBallVelocity(GLint BALL_VELOCITY) {
	GLdouble theta;

	theta = (GLdouble)(rand() % 600 - 300);
	theta = theta * 3.141592 / (180. * 10.);
	GetBall()->root->data->object->SetVelocity((Vector3::normalize(Vector3(sin(theta), cos(theta), 0))) * BALL_VELOCITY);
	//GetBall()->root->data->object->SetVelocity((Vector3::normalize(Vector3(sin(theta), cos(theta), 0)))*BALL_VELOCITY);
}