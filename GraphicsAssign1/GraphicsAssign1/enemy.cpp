#include"Units.h"

GameObjectTree* GetEnemy() {
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

void SetenemyBoxVelocity(int enemyMoveTime, Object* ball) {
	static bool enemyBoxMoveRightFlag=false;
	static bool enemyBoxMoveLeftFlag =false;
	Object* enemy = GetEnemy()->root->data->object;
	if (enemyMoveTime > 3000)
		enemyMoveTime = -5000;
	if (enemyMoveTime > 0)
	{
		if (ball->position.x + ball->width / 2 > enemy->position.x + enemy->width / 2)
			enemyBoxMoveRightFlag = true;
		else if (ball->position.x + ball->width / 2 < enemy->position.x + enemy->width / 2)
			enemyBoxMoveLeftFlag = true;
		if (enemyBoxMoveRightFlag)
			enemy->SetVelocity(BOXVELOCITYTORIGHT);
		else if (enemyBoxMoveLeftFlag)
			enemy->SetVelocity(BOXVELOCITYTOLEFT);
		else
			enemy->SetVelocity(BOXVELOCITYZERO);
		enemyBoxMoveRightFlag = false;
		enemyBoxMoveLeftFlag = false;
	}
	else
	{
		enemy->SetVelocity(BOXVELOCITYZERO);
		enemyBoxMoveRightFlag = false;
		enemyBoxMoveLeftFlag = false;
	}
}