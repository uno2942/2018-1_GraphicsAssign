#include<iostream>
#include<glew.h>
#include<freeglut.h>
#include "Draw.h"

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
//		cout << "Player: " << GameManager::getInstance().playerBox.position << '\t' << "Ball: " << GameManager::getInstance().ball.position << endl;
		glutTimerFunc(1, idlefunction, 0);
	}
}

void myKeyboard(unsigned char c, int x, int y) {
	if (GameManager::getInstance().WhoFinallyWin == 0) {
		switch (c) {
		case '1': camMode = WHOLE; break;
		case '2': camMode = FOLLOWBALL;
		}
	}
}

void specialKeyboard(int key, int x, int y) {
	GameManager::getInstance().SpecialKeyboardInputHandler(key); 
	head();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1600, 900);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Pukimun Bullyball");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(specialKeyboard);
	glutTimerFunc(3, idlefunction, 0);
	glutTimerFunc(17, idle2, 0);
	glutMainLoop();
}