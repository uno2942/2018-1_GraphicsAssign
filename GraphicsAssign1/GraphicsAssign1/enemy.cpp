#include"Units.h"

static GameObjectTree* GetEnemy() {
	static GameObjectTree* enemyTree;
	if(enemyTree == NULL)
	{
		enemyTree = new GameObjectTree("enemyBox");
		Object* enemy = new Box("enemyBox", WORLDCOORDWINDOWWIDTH / 8, WORLDCOORDWINDOWHEIGHT / 18, MY_YELLOW);
		GameObject* enemyObject = new GameObject(enemy);
		GameObjectNode* enemyNode = new GameObjectNode(enemyObject, "enemy");
		enemyObject->AddCollisionComponentAsItself(enemyNode);
		enemyTree->insert_back(enemyNode);
	}
	return enemyTree;
}