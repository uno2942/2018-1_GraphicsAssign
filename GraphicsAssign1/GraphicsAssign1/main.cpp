#include<iostream>
#include<glew.h>
#include<freeglut.h>
#include "Draw.h"

void head() {
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	GameManager::getInstance().OneFramePipeline();
}
void idle2(int value) {
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	cout << "Player: " << GameManager::getInstance().playerBox.position << '\t' << "Ball: " << GameManager::getInstance().ball.position << endl;
	glutPostRedisplay();
	glutTimerFunc(17, idle2, 0);
}

void idlefunction(int value) {
	head();
	glutTimerFunc(3, idlefunction, 0);
}

void specialKeyboard(int key, int x, int y) {
	GameManager::getInstance().SpecialKeyboardInputHandler(key); 
	head();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Pukimun Bullyball");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutSpecialFunc(specialKeyboard);
	glutTimerFunc(3, idlefunction, 0);
	glutTimerFunc(17, idle2, 0);
	glutMainLoop();
}