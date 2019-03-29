#include<ctime>
#include "GameManager.h"
using namespace std;
/**
여기서 srand 함수를 실행시킨다.
**/
void GameManager::deleteTree(GameObjectTree& tree) {
	deleteTree_aux(tree.root);
}
void GameManager::deleteTree_aux(GameObjectNode* root) {
	if (root == NULL) return;
	GameObjectNode* temp1 = root->successor;
	GameObjectNode* temp2 = root->sibling;
	delete root;
	if (temp1 != NULL)
		deleteTree_aux(temp1);
	if (temp2 != NULL)
		deleteTree_aux(temp2);
}

GLdouble GameManager::BALL_VELOCITY = 300;
GameManager::GameManager() {
	srand((unsigned int)time(0));
	CollisionComponent::ConnectCollisionManagerAddFunction(CollisionManager::PutCollisionObject);
		//ball 부분
	{
		ball = new Oval("ball", 100, 100, MY_RED);
		GameObject* ballObject = new GameObject(ball);
		GameObjectNode* ballNode = new GameObjectNode(ballObject, "ball");
		ballObject->AddCollisionComponentAsItself(ballNode);
		ballTree.insert_back(ballNode);

		Object* ele = new Box("elec1", 20, 7, MY_YELLOW, -10);
		eleroot = ele;
		ele->SetPosition(25, 15);
		ele->SetRotationAxis(25, 35);
		GameObject* elegObject = new GameObject(ele);
		electricity.insert_back(elegObject, "ele1");

		ele = new Box("elec2", 20, 7, MY_YELLOW,30);
		ele->SetPosition(20 * cos(10.* 3.1416/180), 20 * sin(10.* 3.1416 / 180)-3);
		elegObject = new GameObject(ele);
		electricity.insert_back(elegObject, "ele2");

		ele = new Box("elec3", 20, 7, MY_YELLOW ,-10);
		ele->SetPosition(20 * cos(10.* 3.1416 / 180), -20 * sin(10.* 3.1416 / 180) + 3);
		elegObject = new GameObject(ele);
		electricity.insert_back(elegObject, "ele3");

		ele = new Box("elec4", 20, 7, MY_YELLOW,30);
		ele->SetPosition(20 * cos(10.* 3.1416 / 180), 20 * sin(10.* 3.1416 / 180) - 3);
		elegObject = new GameObject(ele);
		electricity.insert_back(elegObject, "ele4");

		ballTree.insert_back(&electricity);
	}
	//Player 부분
	{
		player = new Box("playerBox", WORLDCOORDWINDOWWIDTH / 8, WORLDCOORDWINDOWHEIGHT / 18, MY_YELLOW);
		GameObject* playerObject = new GameObject(player);
		GameObjectNode* playerNode = new GameObjectNode(playerObject, "player");
		playerObject->AddCollisionComponentAsItself(playerNode);
		playerTree.insert_back(playerNode);
		
		tail1 = new Triangle("tail1", 50, 50, MY_CYAN, 50);
		GameObject* temp = new GameObject(tail1);
		temp->object->SetRotationAxis(50, 0);
		temp->object->SetPosition(-50, WORLDCOORDWINDOWHEIGHT / 36);
		GameObjectNode* tempNode = new GameObjectNode(temp, "tail1");
		tailTree.insert_back(tempNode);

		tail2 = new Triangle("tail2", 50, 50, MY_CYAN);
		temp = new GameObject(tail2);
		temp->object->SetRotationAxis(50, 0);
		temp->object->SetPosition(-40, WORLDCOORDWINDOWHEIGHT / 36);
		tempNode = new GameObjectNode(temp, "tail2");
		tailTree.insert_back(tempNode);

		playerTree.insert_back(&tailTree);
		ear = new Oval("ear", 25, 50, MY_CYAN, 0);
		temp = new GameObject(ear);
		temp->object->SetRotationAxis(25, 0);
		temp->object->SetPosition(160, WORLDCOORDWINDOWHEIGHT / 18);
		playerTree.insertAsSibling(temp, "ear", "tail1");

	}
	//enemy 부분
	{
		enemy = new Box("enemyBox", WORLDCOORDWINDOWWIDTH / 8, WORLDCOORDWINDOWHEIGHT / 18, MY_YELLOW);
		GameObject* enemyObject = new GameObject(enemy);
		GameObjectNode* enemyNode = new GameObjectNode(enemyObject, "enemy");
		enemyObject->AddCollisionComponentAsItself(enemyNode);
		enemyTree.insert_back(enemyNode);

	}
	//net 부분
	{
		net = new Box("net", WORLDCOORDWINDOWWIDTH / 32, WORLDCOORDWINDOWHEIGHT / 2, MY_GREEN);
		GameObject* netObject = new GameObject(net);
		GameObjectNode* netNode = new GameObjectNode(netObject, "net");
		netObject->AddCollisionComponentAsItself(netNode);
		netTree.insert_back(netNode);
	}

	//cloud1 부분
	{
		Object* cloud;
		cloud1root = new Oval("cluod_mid", 80, 80, MY_CLOUD_COLOR);
		GameObject* cloudObject1 = new GameObject(cloud1root);
		GameObjectNode* cloudNode = new GameObjectNode(cloudObject1, "cloud_mid");
		cloudTree1.insert_back(cloudNode);
		
		cloud1left = new Oval("cluod_left1", 80, 80, MY_CLOUD_COLOR);
		cloud1left->SetRotationAxis(70, 70);
		GameObject* cloudObject2 = new GameObject(cloud1left);
		cloudObject2->object->SetPosition(-30, -30);
		cloudTree1.insertAsChildren(cloudObject2, "cloud_left_1", "cloud_mid");
		
		cloud1right = new Oval("cloud_right_1", 80, 80, MY_CLOUD_COLOR);
		cloud1right->SetRotationAxis(10, 70);
		GameObject* cloudObject3 = new GameObject(cloud1right);
		cloudObject3->object->SetPosition(30, -30);
		cloudTree1.insertAsSibling(cloudObject3, "cloud_right_1", "cloud_left_1");
		
		cloud = new Oval("cloud_left_2", 80, 80, MY_CLOUD_COLOR);
		GameObject* cloudObject4 = new GameObject(cloud);
		cloudObject4->object->SetPosition(-30, 30);
		cloudTree1.insertAsChildren(cloudObject4, "cloud_left_2", "cloud_left_1");
		
		cloud = new Oval("cloud_right_2", 80, 80, MY_CLOUD_COLOR);
		GameObject* cloudObject5 = new GameObject(cloud);
		cloudObject5->object->SetPosition(30, 30);
		cloudTree1.insertAsChildren(cloudObject5, "cloud_right_2", "cloud_right_1");
	}
	
	//cloud2 부분
	{
		Object* cloud;
		cloud2root = new Oval("cluod_mid", 80, 80, MY_CLOUD_COLOR);
		GameObject* cloudObject1 = new GameObject(cloud2root);
		GameObjectNode* cloudNode = new GameObjectNode(cloudObject1, "cloud_mid");
		cloudTree2.insert_back(cloudNode);

		cloud2left = new Oval("cluod_left1", 80, 80, MY_CLOUD_COLOR);
		cloud2left->SetRotationAxis(70, 70);
		GameObject* cloudObject2 = new GameObject(cloud2left);
		cloudObject2->object->SetPosition(-30, -30);
		cloudTree2.insertAsChildren(cloudObject2, "cloud_left_1", "cloud_mid");

		cloud2right = new Oval("cloud_right_1", 80, 80, MY_CLOUD_COLOR);
		cloud2right->SetRotationAxis(10, 70);
		GameObject* cloudObject3 = new GameObject(cloud2right);
		cloudObject3->object->SetPosition(30, -30);
		cloudTree2.insertAsSibling(cloudObject3, "cloud_right_1", "cloud_left_1");

		cloud = new Oval("cloud_left_2", 80, 80, MY_CLOUD_COLOR);
		GameObject* cloudObject4 = new GameObject(cloud);
		cloudObject4->object->SetPosition(-30, 30);
		cloudTree2.insertAsChildren(cloudObject4, "cloud_left_2", "cloud_left_1");

		cloud = new Oval("cloud_right_2", 80, 80, MY_CLOUD_COLOR);
		GameObject* cloudObject5 = new GameObject(cloud);
		cloudObject5->object->SetPosition(30, 30);
		cloudTree2.insertAsChildren(cloudObject5, "cloud_right_2", "cloud_right_1");
	}

	//cloud3 부분
	{
		Object* cloud;
		cloud3root = new Oval("cluod_mid", 80, 80, MY_CLOUD_COLOR);
		GameObject* cloudObject1 = new GameObject(cloud3root);
		GameObjectNode* cloudNode = new GameObjectNode(cloudObject1, "cloud_mid");
		cloudTree3.insert_back(cloudNode);

		cloud3left = new Oval("cluod_left1", 80, 80, MY_CLOUD_COLOR);
		cloud3left->SetRotationAxis(70, 70);
		GameObject* cloudObject2 = new GameObject(cloud3left);
		cloudObject2->object->SetPosition(-30, -30);
		cloudTree3.insertAsChildren(cloudObject2, "cloud_left_1", "cloud_mid");

		cloud3right = new Oval("cloud_right_1", 80, 80, MY_CLOUD_COLOR);
		cloud3right->SetRotationAxis(10, 70);
		GameObject* cloudObject3 = new GameObject(cloud3right);
		cloudObject3->object->SetPosition(30, -30);
		cloudTree3.insertAsSibling(cloudObject3, "cloud_right_1", "cloud_left_1");

		cloud = new Oval("cloud_left_2", 80, 80, MY_CLOUD_COLOR);
		GameObject* cloudObject4 = new GameObject(cloud);
		cloudObject4->object->SetPosition(-30, 30);
		cloudTree3.insertAsChildren(cloudObject4, "cloud_left_2", "cloud_left_1");

		cloud = new Oval("cloud_right_2", 80, 80, MY_CLOUD_COLOR);
		GameObject* cloudObject5 = new GameObject(cloud);
		cloudObject5->object->SetPosition(30, 30);
		cloudTree3.insertAsChildren(cloudObject5, "cloud_right_2", "cloud_right_1");
	}

	//wall(스크린 밖에 안 보이는 벽) 부분
	{
	GameObject* temp = new GameObject(new Box("leftwall", 10, WORLDCOORDWINDOWHEIGHT, MY_WALL_COLOR));
	GameObjectNode* tempNode = new GameObjectNode(temp, "leftwall");
	temp->AddCollisionComponentAsItself(tempNode);
	wallTree.insert_back(temp, "leftwall");

	temp = new GameObject(new Box("rightwall", 10, WORLDCOORDWINDOWHEIGHT, MY_WALL_COLOR));
	tempNode = new GameObjectNode(temp, "rightwall");
	temp->AddCollisionComponentAsItself(tempNode);
	wallTree.insertAsSibling(temp, "rightwall", "leftwall");

	temp = new GameObject(new Box("topwall", WORLDCOORDWINDOWWIDTH, 10, MY_WALL_COLOR));
	tempNode = new GameObjectNode(temp, "topwall");
	temp->AddCollisionComponentAsItself(tempNode);
	wallTree.insertAsSibling(temp, "topwall", "leftwall");

	temp = new GameObject(new Box("leftwallofleft", 300, WORLDCOORDWINDOWHEIGHT));
	temp->object->SetPosition(-300, 0);
	tempNode = new GameObjectNode(temp, "leftwallofleft");
	wallTree.insertAsChildren(temp, "leftwallofleft", "leftwall");

	temp = new GameObject(new Box("rightwallofright", 300, WORLDCOORDWINDOWHEIGHT));
	temp->object->SetPosition(10, 0);
	tempNode = new GameObjectNode(temp, "rightwallofright");
	wallTree.insertAsChildren(temp, "rightwallofright", "rightwall");
	}
	
	objectsTreeVectorForDraw.push_back(playerTree);
	objectsTreeVectorForDraw.push_back(enemyTree);
	objectsTreeVectorForDraw.push_back(netTree);
	objectsTreeVectorForDraw.push_back(cloudTree1);
	objectsTreeVectorForDraw.push_back(cloudTree2);
	objectsTreeVectorForDraw.push_back(cloudTree3);
	objectsTreeVectorForDraw.push_back(wallTree);
	objectsTreeVectorForDraw.push_back(ballTree);
	
	StartGame();
}

GameManager::~GameManager() {
	deleteTree(playerTree);
	deleteTree(enemyTree);
	deleteTree(netTree);
	deleteTree(wallTree);
	deleteTree(ballTree);
	deleteTree(cloudTree1);
	deleteTree(cloudTree2);
	deleteTree(cloudTree3);
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

	cloud1root->SetPosition(INITIAL_CLOUD1_POSITION);
	cloud2root->SetPosition(INITIAL_CLOUD2_POSITION);
	cloud3root->SetPosition(INITIAL_CLOUD3_POSITION);
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
	shakeTime = 0;
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
	if(shakeTime<=0)
		shakeTime = 1000;
}
void GameManager::SetObjectPosition() {
	
	player->position += player->velocity*PLAYER_BOX_VELOCITY;
	enemy->position += enemy->velocity*ENEMY_BOX_VELOCITY;
	ball->position += ball->velocity*(((timeSinceStart - prevTime) / 1000.) + collisionManager.ballDeltaTime);
	eleroot->rotation += 40 * (((timeSinceStart - prevTime) / 1000.) + collisionManager.ballDeltaTime);
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
	
	cloud1root->position += Vector2(80, 0) * (((timeSinceStart - prevTime) / 1000.));
	cloud2root->position += Vector2(80, 0) * (((timeSinceStart - prevTime) / 1000.));
	cloud3root->position += Vector2(80, 0) * (((timeSinceStart - prevTime) / 1000.));

	if (cloud1root->position.x > WORLDCOORDWINDOWWIDTH+200)
		cloud1root->position.x -= WORLDCOORDWINDOWWIDTH+500;

	if (cloud2root->position.x > WORLDCOORDWINDOWWIDTH + 200)
		cloud2root->position.x -= WORLDCOORDWINDOWWIDTH + 500;

	if (cloud3root->position.x > WORLDCOORDWINDOWWIDTH + 200)
		cloud3root->position.x -= WORLDCOORDWINDOWWIDTH + 500;

	if (cloud1left->rotation > 20)
		cloudLeftUp = false;
	else if (cloud1left->rotation < -20)
		cloudLeftUp = true;
	if (cloud1right->rotation > 20)
		cloudRightUp = false;
	else if (cloud1right->rotation < -20)
		cloudRightUp = true;
	
	if (cloudLeftUp)
	{
		cloud1left->rotation += 40 * (((timeSinceStart - prevTime) / 1000.));
		cloud2left->rotation += 40 * (((timeSinceStart - prevTime) / 1000.));
		cloud3left->rotation += 40 * (((timeSinceStart - prevTime) / 1000.));
	}
	else {
		cloud1left->rotation -= 40 * (((timeSinceStart - prevTime) / 1000.));
		cloud2left->rotation -= 40 * (((timeSinceStart - prevTime) / 1000.));
		cloud3left->rotation -= 40 * (((timeSinceStart - prevTime) / 1000.));
	}

	if (cloudRightUp)
	{
		cloud1right->rotation += 40 * (((timeSinceStart - prevTime) / 1000.));
		cloud2right->rotation += 40 * (((timeSinceStart - prevTime) / 1000.));
		cloud3right->rotation += 40 * (((timeSinceStart - prevTime) / 1000.));
	}
	else
	{
		cloud1right->rotation -= 40 * (((timeSinceStart - prevTime) / 1000.));
		cloud2right->rotation -= 40 * (((timeSinceStart - prevTime) / 1000.));
		cloud3right->rotation -= 40 * (((timeSinceStart - prevTime) / 1000.));
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


