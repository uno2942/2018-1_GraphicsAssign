#include<iostream>
#include<glew.h>
#include<freeglut.h>
#include "Draw.h"

/**
�̰� Ű ��ǲ ������ �۵� �� �ϳ���?
**/
void idlefunction() {
	GameManager::getInstance().OneFramePipeline();
	glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y) {
	GameManager::getInstance().SpecialKeyboardInputHandler(key);
	GameManager::getInstance().OneFramePipeline();
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Pukimun Bullyball");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutIdleFunc(idlefunction);
	glutSpecialFunc(specialKeyboard);
	glutMainLoop();
}