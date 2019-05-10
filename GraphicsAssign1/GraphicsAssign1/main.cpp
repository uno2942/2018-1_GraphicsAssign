#include<iostream>
#include<glew.h>
#include<freeglut.h>
#include"GameManager.h"
#include "Draw.h"
#include"Camera.h"
void head() {
	static int prevTime = glutGet(GLUT_ELAPSED_TIME);
	if(glutGet(GLUT_ELAPSED_TIME)-prevTime>5)
		GameManager::getInstance().FreshTime();
	if(!ReshapeFlag && (GameManager::getInstance().WhoFinallyWin==0))
		GameManager::getInstance().OneFramePipeline();
	prevTime = glutGet(GLUT_ELAPSED_TIME);
}

void idle2(int value) {
	glutPostRedisplay();
	glutTimerFunc(17, idle2, 0);
}

void idlefunction(int value) {
	if (GameManager::getInstance().WhoFinallyWin == 0) {
		head();
	}
	glutTimerFunc(1, idlefunction, 0);
}

void myKeyboard(unsigned char c, int x, int y) {
	GLfloat a = 0;
	GLfloat b = 0;
	if (c == 'r')
		GameManager::getInstance().StartGame();
	if (GameManager::getInstance().WhoFinallyWin == 0) {
		switch (c) {
		case '1': camMode = CHARACTER; break;
		case '2': camMode = BEHIND; break;
		case '3': camMode = HANGING;
			cin >> a >> b;
			myCamera::SetHangingxy(a, b);
			GameManager::getInstance().FreshTime();
			break;
		case '4': renMode = GOURAUD; break;
		case '5': renMode = PHONG;

		}
	}
}

void specialKeyboard(int key, int x, int y) {
	GameManager::getInstance().SpecialKeyboardInputHandler(key); 
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1600, 900);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Pukimun Bullyball");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(specialKeyboard);
	glutTimerFunc(1, idlefunction, 0);
	glutTimerFunc(17, idle2, 0);
	glutMainLoop();
}