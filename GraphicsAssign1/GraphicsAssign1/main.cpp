#include<iostream>
#include <glew.h>
#include <freeglut.h>
#include "Draw.h"

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