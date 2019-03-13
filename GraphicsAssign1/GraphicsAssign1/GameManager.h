#pragma once
#include"Objects.h"
class GameManager {
public:
	class SceneManager {

	};

	class CollisionManager {

	};

	GameManager();
	/**
	Velocity를 바꿀지 position을 바꿀지 생각
	**/
	void SpecialKeyboardInputHandler(int key);
private:
	//Manager
	SceneManager sceneManager;
	CollisionManager collisionManager;

	//ObjectData
	Box playerBox;
	Box enemyBox;
	Circle ball;

	int myScore = 0;
	int enemyScore = 0;

	const Vector2 BOXVELOCITYTORIGHT = Vector2(1, 0);
	const Vector2 BOXVELOCITYTOLEFT = Vector2(-1, 0);
	const Vector2 BOXVELOCITYZERO = Vector2(0, 0);
	/* need to set(dummy data)*/

	const Vector2 InitialplayerBoxPosition = Vector2(1, 0);
	const Vector2 InitialenemyBoxPosition = Vector2(-1, 0);
	const Vector2 InitialBallPosition = Vector2(0, 0);

	bool playerBoxMoveRightFlag = false;
	bool playerBoxMoveLeftFlag = false;

	void StartGame();
	void InitBallVelocity();
	void InitObjectPosition();
	void SetplayerBoxVelocity();
	void SetObjectPosition();
};