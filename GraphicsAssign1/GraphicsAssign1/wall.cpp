#include"Units.h"

GameObjectTree* GetLeftWall() {
	static GameObjectTree* LeftwallTree = NULL;
	if (LeftwallTree == NULL)
	{
		LeftwallTree = new GameObjectTree("leftwall");
		GameObject* temp = new GameObject(new Box("leftwall", 0, WORLD_COORD_MAP_ZLEN, WORLD_COORD_MAP_YLEN, MY_WALL_COLOR));
		temp->object->SetPosition(0, WORLD_COORD_MAP_ZLEN / 2, WORLD_COORD_MAP_YLEN / 2);
		temp->object->rotationAxis = Vector3(0, 0, 1);
		GameObjectNode* tempNode = new GameObjectNode(temp, "leftwall");
		//		temp->AddCollisionComponentAsItself(tempNode);
		LeftwallTree->insert_back(temp, "leftwall");
	}
	return LeftwallTree;
}

GameObjectTree* GetRightWall() {
	static GameObjectTree* RightwallTree = NULL;
	if (RightwallTree == NULL)
	{
		RightwallTree = new GameObjectTree("rightwall");
		GameObject* temp = new GameObject(new Box("rightwall", 0, WORLD_COORD_MAP_ZLEN, WORLD_COORD_MAP_YLEN, MY_WALL_COLOR));
		temp->object->SetPosition(WORLD_COORD_MAP_XLEN, WORLD_COORD_MAP_ZLEN / 2, WORLD_COORD_MAP_YLEN / 2);
		temp->object->rotationAxis = Vector3(0, 0, 1);
		GameObjectNode* tempNode = new GameObjectNode(temp, "rightwall");
//		temp->AddCollisionComponentAsItself(tempNode);
		RightwallTree->insert_back(temp, "rightwall");
	}
	return RightwallTree;
}

GameObjectTree* GetFrontWall() {
	static GameObjectTree* FrontwallTree = NULL;
	if (FrontwallTree == NULL)
	{
		FrontwallTree = new GameObjectTree("frontwall");
		GameObject* temp = new GameObject(new Box("frontwall", WORLD_COORD_MAP_XLEN, 0, WORLD_COORD_MAP_YLEN, MY_WALL_COLOR));
		temp->object->SetPosition(WORLD_COORD_MAP_XLEN / 2, 0, WORLD_COORD_MAP_YLEN / 2);
		temp->object->rotationAxis = Vector3(0, 0, 1);
		GameObjectNode * tempNode = new GameObjectNode(temp, "frontwall");
		//		temp->AddCollisionComponentAsItself(tempNode);
		FrontwallTree->insert_back(temp, "frontwall");
	}
	return FrontwallTree;
}

GameObjectTree* GetBackWall() {
	static GameObjectTree* BackwallTree = NULL;
	if (BackwallTree == NULL)
	{
		BackwallTree = new GameObjectTree("backwall");
		GameObject* temp = new GameObject(new Box("backwall", WORLD_COORD_MAP_XLEN, 0, WORLD_COORD_MAP_YLEN, MY_WALL_COLOR));
		temp->object->SetPosition(WORLD_COORD_MAP_XLEN / 2, WORLD_COORD_MAP_ZLEN, WORLD_COORD_MAP_YLEN / 2);
		temp->object->rotationAxis = Vector3(0, 0, 1);
		GameObjectNode * tempNode = new GameObjectNode(temp, "backwall");
		//		temp->AddCollisionComponentAsItself(tempNode);
		BackwallTree->insert_back(temp, "backwall");
	}
	return BackwallTree;
}

GameObjectTree* GetBottomWall() {
	static GameObjectTree* BottomwallTree = NULL;
	if (BottomwallTree == NULL)
	{
		BottomwallTree = new GameObjectTree("bottomwall");
		GameObject* temp = new GameObject(new Box("bottomwall", WORLD_COORD_MAP_XLEN, WORLD_COORD_MAP_ZLEN, 0, MY_WALL_COLOR));
		temp->object->SetPosition(WORLD_COORD_MAP_XLEN / 2, WORLD_COORD_MAP_ZLEN / 2, 0);
		temp->object->rotationAxis = Vector3(0, 0, 1);
		GameObjectNode* tempNode = new GameObjectNode(temp, "bottomwall");
		//		temp->AddCollisionComponentAsItself(tempNode);
		BottomwallTree->insert_back(temp, "bottomwall");
	}
	return BottomwallTree;
}