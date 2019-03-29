#include<ctime>
#include "GameManager.h"
using namespace std;
/**
여기서 srand 함수를 실행시킨다.
**/

GLdouble GameManager::BALL_VELOCITY = 300;
GameManager::GameManager() {
	srand((unsigned int)time(0));
	CollisionComponent::ConnectCollisionManagerAddFunction(CollisionManager::PutCollisionObject);
		//ball 부분
	{
		ball = new Oval("ball", 100, 100);
		ball->SetRotationAxis(50, 50);
		GameObject* ballObject = new GameObject(ball);
		GameObjectNode* ballNode = new GameObjectNode(ballObject, "ball");
		ballObject->AddCollisionComponentAsItself(ballNode);
		ballTree.insert_back(ballNode);
		//electricity 그려야 함.
	}
	//Player 부분
	{
		player = new Box("playerBox", WORLDCOORDWINDOWWIDTH / 8, WORLDCOORDWINDOWHEIGHT / 18);
		GameObject* playerObject = new GameObject(player);
		GameObjectNode* playerNode = new GameObjectNode(playerObject, "player");
		playerObject->AddCollisionComponentAsItself(playerNode);
		playerTree.insert_back(playerNode);
		
		tail1 = new Triangle("tail1", 50, 50, 50);
		GameObject* temp = new GameObject(tail1);
		temp->object->SetRotationAxis(50, 0);
		temp->object->SetPosition(-50, WORLDCOORDWINDOWHEIGHT / 36);
		GameObjectNode* tempNode = new GameObjectNode(temp, "tail1");
		tailTree.insert_back(tempNode);

		tail2 = new Triangle("tail2", 50, 50);
		temp = new GameObject(tail2);
		temp->object->SetRotationAxis(50, 0);
		temp->object->SetPosition(-40, WORLDCOORDWINDOWHEIGHT / 36);
		tempNode = new GameObjectNode(temp, "tail2");
		tailTree.insert_back(tempNode);

		playerTree.insert_back(&tailTree);
		ear = new Oval("ear", 200, 200, 0);
		temp = new GameObject(ear);
		temp->object->SetRotationAxis(25, 0);
		temp->object->SetPosition(-50, WORLDCOORDWINDOWHEIGHT / 36);
		playerTree.insertAsSibling(temp, "ear", "tail1");

	}
	//enemy 부분
	{
		enemy = new Box("enemyBox", WORLDCOORDWINDOWWIDTH / 8, WORLDCOORDWINDOWHEIGHT / 18);
		GameObject* enemyObject = new GameObject(enemy);
		GameObjectNode* enemyNode = new GameObjectNode(enemyObject, "enemy");
		enemyObject->AddCollisionComponentAsItself(enemyNode);
		enemyTree.insert_back(enemyNode);

	}
	//net 부분
	{
		net = new Box("net", WORLDCOORDWINDOWWIDTH / 32, WORLDCOORDWINDOWHEIGHT / 2);
		GameObject* netObject = new GameObject(net);
		GameObjectNode* netNode = new GameObjectNode(netObject, "net");
		netObject->AddCollisionComponentAsItself(netNode);
		netTree.insert_back(netNode);
	}

	//cloud 부분
	{
		cloud1 = new Oval("cluod_mid", 80, 80);
		GameObject* cloudObject1 = new GameObject(cloud1);
		cloudObject1->object->SetPosition(500, 700);
		GameObjectNode* cloudNode = new GameObjectNode(cloudObject1, "cloud_mid");
		cloudTree1.insert_back(cloudNode);
		
		cloud1 = new Oval("cluod_mid", 80, 80);
		GameObject* cloudObject2 = new GameObject(cloud1);
		cloudObject2->object->SetPosition(-60, -60);
		cloudTree1.insertAsChildren(cloudObject2, "cloud_left_1", "cloud_mid");
		
		cloud1 = new Oval("cluod_mid", 80, 80);
		GameObject* cloudObject3 = new GameObject(cloud1);
		cloudObject3->object->SetPosition(60, -60);
		cloudTree1.insertAsSibling(cloudObject3, "cloud_right_1", "cloud_left_1");
		
		cloud1 = new Oval("cluod_mid", 80, 80);
		GameObject* cloudObject4 = new GameObject(cloud1);
		cloudObject4->object->SetPosition(-60, 60);
		cloudObject4->object->SetRotationAxis(28, -28);
		cloudTree1.insertAsChildren(cloudObject4, "cloud_left_2", "cloud_left_1");
		
		cloud1 = new Oval("cluod_mid", 80, 80);
		GameObject* cloudObject5 = new GameObject(cloud1);
		cloudObject5->object->SetPosition(60, 60);
		cloudObject5->object->SetRotationAxis(-28, -28);
		cloudTree1.insertAsChildren(cloudObject5, "cloud_right_2", "cloud_right_1");
		
		printf("hi");
	}
	


	//wall(스크린 밖에 안 보이는 벽) 부분
	{
	GameObject* temp = new GameObject(new Box("leftwall", 10, WORLDCOORDWINDOWHEIGHT));
	GameObjectNode* tempNode = new GameObjectNode(temp, "leftwall");
	temp->AddCollisionComponentAsItself(tempNode);
	wallTree.insert_back(temp, "leftwall");

	temp = new GameObject(new Box("rightwall", 10, WORLDCOORDWINDOWHEIGHT));
	tempNode = new GameObjectNode(temp, "rightwall");
	temp->AddCollisionComponentAsItself(tempNode);
	wallTree.insert_back(temp, "rightwall");

	temp = new GameObject(new Box("topwall", 10, WORLDCOORDWINDOWHEIGHT));
	tempNode = new GameObjectNode(temp, "topwall");
	temp->AddCollisionComponentAsItself(tempNode);
	wallTree.insert_back(temp, "topwall");
	}
	
	objectsTreeVectorForDraw.push_back(playerTree);
	objectsTreeVectorForDraw.push_back(enemyTree);
	objectsTreeVectorForDraw.push_back(netTree);
	objectsTreeVectorForDraw.push_back(wallTree);
	objectsTreeVectorForDraw.push_back(cloudTree1);
	objectsTreeVectorForDraw.push_back(ballTree);
	
	StartGame();
}

void GameManager::OneFramePipeline() {
	timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	SetplayerBoxVelocity();
	SetenemyBoxVelocity();
	collisionManager.CollisionHandler(collisionManager.RestoreBallPosition(collisionManager.CollisionCheck()));
	SetObjectPosition();
	prevTime = timeSinceStart;
}

void GameManager::SpecialKeyboardInputHandler(int key) {
	switch (key) {
	case GLUT_KEY_RIGHT: playerBoxMoveRightFlag = true; break;
	case GLUT_KEY_LEFT: playerBoxMoveLeftFlag = true;
	}
}

void GameManager::StartGame() {
	myScore = 0;
	enemyScore = 0;
	WhoFinallyWin = 0;
	InitializeGame();
	/*...*/
}

void GameManager::InitializeGame() {
	oneGameEnd = false;
	enemyMoveTime = 0;
	InitObjectsPosition();
	InitBallVelocity();
	timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	prevTime = glutGet(GLUT_ELAPSED_TIME);
}

/**
World coord. 기준
**/
void GameManager::InitObjectsPosition() {
	(*player).SetPosition(INITIAL_PLAYER_BOX_POSITION);
	(*enemy).SetPosition(INITIAL_ENEMY_BOX_POSITION);
	(*ball).SetPosition(INITIAL_BALL_POSITION);
	netTree.root->data->object->SetPosition(INITIAL_NET_POSITION);
	wallTree.root->data->object->SetPosition(INITIAL_LEFT_WALL_POSITION);
	wallTree.Find("rightwall")->data->object->SetPosition(INITIAL_RIGHT_WALL_POSITION);
	wallTree.Find("topwall")->data->object->SetPosition(INITIAL_TOP_WALL_POSITION);

	tail1->SetRotation(50);
	tail2->SetRotation(0);
	ear->SetRotation(0);
}

/**
게임 시작 시 공의 초기 속도를 결정해주는 함수
**/

void GameManager::InitBallVelocity() {
	BALL_VELOCITY = 300;
	int x = 0;
	int y = 0;
	
	while(0==x)
		x = rand()%101-50;

	while (-20 < y && y < 20)
		y = rand() % 101 - 50;

	ball->SetVelocity((Vector2::normalize(Vector2(x, y)))*BALL_VELOCITY);
}


void GameManager::SetplayerBoxVelocity() {
	if (playerBoxMoveRightFlag)
		player->SetVelocity(BOXVELOCITYTORIGHT);
	else if (playerBoxMoveLeftFlag)
		player->SetVelocity(BOXVELOCITYTOLEFT);
	else
		player->SetVelocity(BOXVELOCITYZERO);
	playerBoxMoveRightFlag = false;
	playerBoxMoveLeftFlag = false;
}

void GameManager::SetenemyBoxVelocity() {
	if (enemyMoveTime > 3000)
		enemyMoveTime = -5000;
	enemyMoveTime += DeltaTime();
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
void GameManager::SetShakeTime() {
	shakeTime = 1000;
}
void GameManager::SetObjectPosition() {
	player->position += player->velocity*PLAYER_BOX_VELOCITY;
	enemy->position += enemy->velocity*ENEMY_BOX_VELOCITY;
	ball->position += ball->velocity*(((timeSinceStart - prevTime) / 1000.) + collisionManager.ballDeltaTime);
	ball->rotation += 40 * (((timeSinceStart - prevTime) / 1000.) + collisionManager.ballDeltaTime);
	if (shakeTime > 0) {
		//tail1
		{
			if (tail1->rotation >= 70)
			{
				tail1upflag = false;
				tail1downflag = true;
			}
			else if (tail1->rotation <= 50)
			{
				tail1upflag = true;
				tail1downflag = false;
			}
			if (tail1upflag)
				tail1->rotation += 40 * (((timeSinceStart - prevTime) / 1000.));
			else if (tail1downflag)
				tail1->rotation -= 40 * (((timeSinceStart - prevTime) / 1000.));
		}
		//tail2
		{
			if (tail2->rotation >= 20)
			{
				tail2upflag = false;
				tail2downflag = true;
			}
			else if (tail2->rotation <= -20)
			{
				tail2upflag = true;
				tail2downflag = false;
			}
			if (tail2upflag)
				tail2->rotation += 80 * (((timeSinceStart - prevTime) / 1000.));
			else if (tail2downflag)
				tail2->rotation -= 80 * (((timeSinceStart - prevTime) / 1000.));
		}
		//ear
		{
			if (ear->rotation >= 20)
			{
				earleftflag = false;
				earrightflag = true;
			}
			else if (ear->rotation <= -20)
			{
				earleftflag = true;
				earrightflag = false;
			}
			if (earleftflag)
				ear->rotation += 80 * (((timeSinceStart - prevTime) / 1000.));
			else if (earrightflag)
				ear->rotation -= 80 * (((timeSinceStart - prevTime) / 1000.));
		}
		shakeTime -= (timeSinceStart - prevTime);
	}
	

}

void GameManager::OneGameEnd(bool whoWin) {
	if (whoWin)
		myScore++;
	else
		enemyScore++;
	oneGameEnd = true;
	if (myScore == THRESHOLDSCORE)
		WhoFinallyWin = 1;
	else if (enemyScore == THRESHOLDSCORE)
		WhoFinallyWin = 2;
	InitializeGame();
}


