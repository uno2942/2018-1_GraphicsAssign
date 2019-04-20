#include"Units.h"

GameObjectTree* GetPlayer() {
	static GameObjectTree* playerTree;
	if (playerTree == NULL)
	{
		playerTree = new GameObjectTree("player");
		Object* player = new Box("playerBox", WORLD_COORD_MAP_XLEN / 8, WORLD_COORD_MAP_XLEN / 8, WORLD_COORD_MAP_YLEN / 18, MY_YELLOW);
		GameObject* playerObject = new GameObject(player);
		GameObjectNode* playerNode = new GameObjectNode(playerObject, "player");
		playerObject->AddCollisionComponentAsItself(playerNode);
		playerTree->insert_back(playerNode);
	}
	return playerTree;
}

void SetplayerBoxVelocity(bool playerBoxMoveRightFlag, bool playerBoxMoveLeftFlag) {
	if (playerBoxMoveRightFlag)
		GetPlayer()->root->data->object->SetVelocity(BOXVELOCITYTORIGHT);
	else if (playerBoxMoveLeftFlag)
		GetPlayer()->root->data->object->SetVelocity(BOXVELOCITYTOLEFT);
	else
		GetPlayer()->root->data->object->SetVelocity(BOXVELOCITYZERO);
}