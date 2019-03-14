#include<iostream>
#include<glew.h>
#include<freeglut.h>
#include "Draw.h"

void head() {
	GameManager::getInstance().OneFramePipeline();
}

void idle2(int value) {
	glutPostRedisplay();
	glutTimerFunc(17, idle2, 0);
}

void idlefunction(int value) {
	head();
	glutTimerFunc(1, idlefunction, 0);
}

void myKeyboard(unsigned char c, int x, int y) {
	switch (c) {
	case '1': camMode = WHOLE;
	case '2': camMode = FOLLOWBALL;
	}
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
	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(specialKeyboard);
	glutTimerFunc(3, idlefunction, 0);
	glutTimerFunc(17, idle2, 0);
	glutMainLoop();
}