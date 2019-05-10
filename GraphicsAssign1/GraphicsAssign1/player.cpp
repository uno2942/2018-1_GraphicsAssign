#include"Units.h"

static Object* player;
GameObjectTree* GetPlayer() {
	static GameObjectTree* playerTree = NULL;
	if (playerTree == NULL)
	{
		playerTree = new GameObjectTree("player");
		player = new Box("player", 62.775997161865234, 188.03649902343750, 304.41650390625000, MY_YELLOW);
		GameObject* playerObject = new GameObject(player);
		GameObjectNode* playerNode = new GameObjectNode(playerObject, "player");
		playerObject->AddCollisionComponentAsItself(playerNode);
		playerTree->insert_back(playerNode);

	}
	return playerTree;
}

void SetplayerBoxVelocity(bool playerBoxMoveFrontFlag, bool playerBoxMoveBackFlag) {
	GLdouble theta = player->GetCurrentRotation();
	if (playerBoxMoveFrontFlag)
		player->SetVelocity(Vector3(sin(theta), cos(theta), 0));
	else if (playerBoxMoveBackFlag)
		player->SetVelocity(-Vector3(sin(theta), cos(theta), 0));
	else
		player->SetVelocity(BOXVELOCITYZERO);
}

void SetplayerBoxRotation(bool playerBoxRotateCounterClockwiseFlag, bool playerBoxRotateClockwiseFlag) {
	if (playerBoxRotateCounterClockwiseFlag == true)
		player->rotation -= 0.03;
	else if (playerBoxRotateClockwiseFlag == true)
		player->rotation += 0.03;
}