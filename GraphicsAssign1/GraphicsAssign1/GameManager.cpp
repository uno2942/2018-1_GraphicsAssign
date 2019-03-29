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

		GameObject* temp = new GameObject(new Triangle("tail1", 150, 150, 153));
		GameObjectNode* tempNode = new GameObjectNode(temp, "tail1");
		tailTree.insert_back(tempNode);
		playerTree.insert_back(&tailTree);

		//ear를 그려야 하는데 보면서 그려야 할 듯...

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

void GameManager::SetObjectPosition() {
	player->position += player->velocity*PLAYER_BOX_VELOCITY;
	enemy->position += enemy->velocity*ENEMY_BOX_VELOCITY;
	ball->position += ball->velocity*(((timeSinceStart - prevTime) / 1000.) + collisionManager.ballDeltaTime);
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


