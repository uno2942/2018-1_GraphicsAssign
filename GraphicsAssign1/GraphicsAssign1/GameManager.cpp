#include<ctime>
#include "GameManager.h"
using namespace std;
/**
여기서 srand 함수를 실행시킨다.
**/

GLdouble GameManager::BALL_VELOCITY = 300;
GameManager::GameManager() {
	srand((unsigned int)time(0));
	//Player 부분
	{
		player = new Box("playerBox", WORLDCOORDWINDOWWIDTH / 8, WORLDCOORDWINDOWHEIGHT / 18);
		playerTree.insert_back(player, "playerBox");
		tailTree.insert_back(new Triangle("tail1", 10, 10, 180), "tail1");
		tailTree.insert_back(new Triangle("tail2", 10, 10, -30), "tail2");
		playerTree.insert_back(&tailTree);

		//ear를 그려야 하는데 보면서 그려야 할 듯...
		collisionManager.PutCollisionObject(playerTree.root, *playerTree.root->data);
	}
	//enemy 부분
	{
		enemy = new Box("enemyBox", WORLDCOORDWINDOWWIDTH / 8, WORLDCOORDWINDOWHEIGHT / 18);
		enemyTree.insert_back(enemy, "enemyBox");
		collisionManager.PutCollisionObject(enemyTree.root, *enemyTree.root->data);
	}
	//net 부분
	{
		net = new Box("net", WORLDCOORDWINDOWWIDTH / 32, WORLDCOORDWINDOWHEIGHT / 2);
		netTree.insert_back(ball, "net");
		collisionManager.PutCollisionObject(netTree.root, *netTree.root->data);
	}
	//wall(스크린 밖에 안 보이는 벽) 부분
	{wallTree.insert_back(new Box("leftwall", 10, WORLDCOORDWINDOWHEIGHT), "leftwall");

	collisionManager.PutCollisionObject(wallTree.root, *wallTree.root->data);
	wallTree.insertAsSibling(new Box("rightwall", 10, WORLDCOORDWINDOWHEIGHT), "rightwall", "leftwall");
	collisionManager.PutCollisionObject(wallTree.Find("rightwall"), *wallTree.Find("rightwall")->data);
	wallTree.insertAsSibling(new Box("topwall", WORLDCOORDWINDOWWIDTH, 10), "topwall", "leftwall");
	collisionManager.PutCollisionObject(wallTree.Find("topwall"), *wallTree.Find("topwall")->data);
	}
	//ball 부분
	{
		ball = new Oval("ball", 100, 100);
		ballTree.insert_back(ball, "ball");
		//electricity 그려야 함.
		collisionManager.PutCollisionObject(ballTree.root, *ballTree.root->data);
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

void GameManager::InitializeGame() {
	oneGameEnd = false;
	enemyMoveTime = 0;
	InitObjectsPosition();
	InitBallVelocity();
	timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	prevTime = glutGet(GLUT_ELAPSED_TIME);
}
void GameManager::StartGame() {
	myScore = 0;
	enemyScore = 0;
	WhoFinallyWin = 0;
	InitializeGame();
	/*...*/
}
/**
World coord. 기준
**/
void GameManager::InitObjectsPosition() {
	(*player).SetPosition(INITIAL_PLAYER_BOX_POSITION);
	(*enemy).SetPosition(INITIAL_ENEMY_BOX_POSITION);
	(*ball).SetPosition(INITIAL_BALL_POSITION);
	netTree.root->data->SetPosition(INITIAL_NET_POSITION);
	wallTree.root->data->SetPosition(INITIAL_LEFT_WALL_POSITION);
	wallTree.Find("rightwall")->data->SetPosition(INITIAL_RIGHT_WALL_POSITION);
	wallTree.Find("topwall")->data->SetPosition(INITIAL_TOP_WALL_POSITION);
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


