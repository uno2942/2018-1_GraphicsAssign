#include"Units.h"

GameObjectTree* GetEnemy() {
	static GameObjectTree* enemyTree = NULL;
	if (enemyTree == NULL)
	{
		enemyTree = new GameObjectTree("enemy");
		Object* enemy = new Box("enemy", 260.0662615, 94.534015, 353.356514, MY_YELLOW);
		GameObject* enemyObject = new GameObject(enemy);
		GameObjectNode* enemyNode = new GameObjectNode(enemyObject, "enemy");
		enemyObject->AddCollisionComponentAsItself(enemyNode);
		enemyTree->insert_back(enemyNode);
	}
	return enemyTree;
}

void SetenemyBoxVelocity(GLint* enemyMoveTime, Object* ball) {
	static bool enemyBoxMoveRightFlag = false;
	static bool enemyBoxMoveLeftFlag = false;
	Object* enemy = GetEnemy()->root->data->object;
	if (*enemyMoveTime > 3000)
		*enemyMoveTime = -5000;
	if (*enemyMoveTime > 0)
	{
		if (ball->position.x > enemy->position.x)
			enemyBoxMoveRightFlag = true;
		else if (ball->position.x < enemy->position.x)
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