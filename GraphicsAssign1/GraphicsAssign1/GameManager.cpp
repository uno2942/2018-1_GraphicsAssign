#include "Objects.h"
class GameManager {
public:
	class SceneManager {

	};

	class CollisionManager {

	};
private:
//Manager
	SceneManager sceneManager;
	CollisionManager collisionManager;

//Data
	Box playerBox;
	Box enemyBox;
	Circle ball;
};