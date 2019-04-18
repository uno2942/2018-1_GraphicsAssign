#include"Units.h"

static GameObjectTree* GetPlayer() {
	static GameObjectTree* playerTree;
	if (playerTree == NULL)
	{
		playerTree = new GameObjectTree("player");
		Object* player = new Box("playerBox", WORLDCOORDWINDOWWIDTH / 8, WORLDCOORDWINDOWHEIGHT / 18, MY_YELLOW);
		GameObject* playerObject = new GameObject(player);
		GameObjectNode* playerNode = new GameObjectNode(playerObject, "player");
		playerObject->AddCollisionComponentAsItself(playerNode);
		playerTree->insert_back(playerNode);
	}
	return playerTree;
}