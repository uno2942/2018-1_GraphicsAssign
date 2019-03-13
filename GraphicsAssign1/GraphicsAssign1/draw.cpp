#include"Draw.h"

void Draw::specialKeyboard(int key, int x, int y) {
		GameManager::getInstance().SpecialKeyboardInputHandler(key);
}