#include"Units.h"

GameObjectTree* GetWall() {
	static GameObjectTree* wallTree;
	if (wallTree == NULL)
	{
		GameObject* temp = new GameObject(new Box("leftwall", 10, WORLD_COORD_MAP_ZLEN, WORLD_COORD_MAP_YLEN, MY_WALL_COLOR));
		GameObjectNode* tempNode = new GameObjectNode(temp, "leftwall");
		temp->AddCollisionComponentAsItself(tempNode);
		wallTree->insert_back(temp, "leftwall");

		temp = new GameObject(new Box("rightwall", 10, WORLD_COORD_MAP_ZLEN, WORLD_COORD_MAP_YLEN, MY_WALL_COLOR));
		tempNode = new GameObjectNode(temp, "rightwall");
		temp->AddCollisionComponentAsItself(tempNode);
		wallTree->insertAsSibling(temp, "rightwall", "leftwall");

		temp = new GameObject(new Box("topwall", WORLD_COORD_MAP_XLEN, WORLD_COORD_MAP_ZLEN, 10, MY_WALL_COLOR));
		tempNode = new GameObjectNode(temp, "topwall");
		temp->AddCollisionComponentAsItself(tempNode);
		wallTree->insertAsSibling(temp, "topwall", "leftwall");

		/*
		temp = new GameObject(new Box("leftwallofleft", 300, WORLDCOORDWINDOWHEIGHT));
		temp->object->SetPosition(-300, 0);
		tempNode = new GameObjectNode(temp, "leftwallofleft");
		wallTree->insertAsChildren(temp, "leftwallofleft", "leftwall");

		temp = new GameObject(new Box("rightwallofright", 300, WORLDCOORDWINDOWHEIGHT));
		temp->object->SetPosition(10, 0);
		tempNode = new GameObjectNode(temp, "rightwallofright");
		wallTree->insertAsChildren(temp, "rightwallofright", "rightwall");
		*/
	}
	return wallTree;
}